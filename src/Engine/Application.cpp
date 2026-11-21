#include "Engine/Application.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>
#include <thread>
#include <utility>



namespace Engine {

Application::Application(ApplicationConfig config)
    : m_window(1600, 900, "Game Engine 72"),
      m_cube(Primitives::createCube()),
      m_sphere(Primitives::createSphere()),
      m_shader("src/Engine/Renderer/shaders/cube.vert",
               "src/Engine/Renderer/shaders/cube.frag"),
      m_robot(m_cube, m_sphere, m_shader, Vec3(0, 0, 0)),
      m_camera({0.0f, 5.0f, 10.0f}),
      m_world(),
      m_config(std::move(config)),
      m_PhysicsWorld()
{
}

void Application::run()
{
    glEnable(GL_DEPTH_TEST);

    m_window.CaptureMouse();

    std::cout << "Starting " << m_config.name << '\n';

    


    auto lastFrameTime = std::chrono::steady_clock::now();
    while (m_running) {
        m_profiler.BeginFrame();

        m_window.PollEvents();

        const auto currentFrameTime = std::chrono::steady_clock::now();
        const std::chrono::duration<float> frameDuration = currentFrameTime - lastFrameTime;
        const float deltaTime = frameDuration.count();
        lastFrameTime = currentFrameTime;
        
        m_profiler.BeginUpdate();
        update(deltaTime);
        m_profiler.EndUpdate();

        m_profiler.BeginRender();
        render();
        m_profiler.EndRender();

        ++m_frame;
        if (/*m_frame >= m_config.maxFrames ||*/ m_window.ShouldClose()) {
            m_running = false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));

        m_profiler.EndFrame();
    }

    std::cout << "Application stopped.\n";
}

void Application::update(float deltaTime)
{
    float speed = 5.0f;

    if (m_WaitingForSecondWTap) {
        m_wTapTimer -= deltaTime;

        if (m_wTapTimer <= 0.0f) {
            m_WaitingForSecondWTap = false;
        }
    }

    if (m_window.WasKeyPressed(GLFW_KEY_W)) {
        if (m_WaitingForSecondWTap) {
            m_camera.SetSprinting(true);
            m_WaitingForSecondWTap = false;
        } else {
            m_WaitingForSecondWTap = true;
            m_wTapTimer = 0.25f;
        }
    }

    if (!m_window.IsKeyPressed(GLFW_KEY_W)) {
        m_camera.SetSprinting(false);
    }

    if (m_window.IsKeyPressed(GLFW_KEY_W)) {
        m_camera.MoveForward(speed * deltaTime);
    }

    if (m_window.IsKeyPressed(GLFW_KEY_S)) {
        m_camera.MoveBackward(speed * deltaTime);
    }

    if (m_window.IsKeyPressed(GLFW_KEY_D)) {
        m_camera.MoveRight(speed * deltaTime);
    }

    if (m_window.IsKeyPressed(GLFW_KEY_A)) {
        m_camera.MoveLeft(speed * deltaTime);
    }

    if (m_window.IsKeyPressed(GLFW_KEY_ESCAPE)) {
        m_running = false;
    }

    if (m_window.WasKeyPressed(GLFW_KEY_F1) || m_window.WasKeyPressed(GLFW_KEY_TAB)) {
        m_window.ToggleMouseCapture();
    }

    float robotSpeed = 9.0f;

    if (m_window.IsKeyPressed(GLFW_KEY_UP)) {
        m_robot.moveForward(robotSpeed * deltaTime, m_world);
    }

    if (m_window.IsKeyPressed(GLFW_KEY_DOWN)) {
        m_robot.moveBackward(robotSpeed * deltaTime, m_world);
    }

    if (m_window.IsKeyPressed(GLFW_KEY_LEFT)) {
        m_robot.rotate(2.0f * deltaTime);
    }

    if (m_window.IsKeyPressed(GLFW_KEY_RIGHT)) {
        m_robot.rotate(-2.0f * deltaTime);
    }

    m_robot.update(deltaTime, m_PhysicsWorld, m_world);

    if (m_window.IsMouseCaptured()) {
        Vec3 MouseDelta = m_window.GetMouseDelta();
        float sensitivity = 0.0010f;

        m_camera.Rotate(-MouseDelta.x * sensitivity, -MouseDelta.y * sensitivity);
    }

    m_cubeRotation += 1.0f * deltaTime;
}

void Application::render()
{
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader.bind();

    Mat4 TerrainModel;
    m_shader.setMat4("model", TerrainModel);
    m_shader.setVec3("color", Vec3(0.1f, 0.7f, 0.1f));
    m_shader.setVec3("lightPosition", Vec3(100.0f, 100.0f, 100.0f));

   

    Mat4 view = m_camera.getViewMatrix();
    m_shader.setMat4("view", view);

    int width;
    int height;

    m_window.GetWindowSize(width, height);

    float aspect = static_cast<float>(width) / static_cast<float>(height);

     Mat4 projection = Mat4::Perspective(70.0f * 3.14159265 / 180.0f, aspect, 0.1f, 1000.0f);
     m_shader.setMat4("projection", projection);

     m_world.draw();

    for (const Obstacle& obstacle : m_world.getObstacles()) {
          Mat4 obstacleModel =
               Mat4::Translation(
                    obstacle.position.x,
                    obstacle.position.y,
                    obstacle.position.z
               )
               *
               Mat4::Scale(
                    obstacle.scale.x,
                    obstacle.scale.y,
                    obstacle.scale.z
               );

          m_shader.setMat4("model", obstacleModel);
          m_shader.setVec3("color", Vec3(0.9f, 0.2f, 0.1f));

          m_cube.draw();
     }

     m_robot.draw();

     const RaySensor& sensor = m_robot.getRaySensor();
     const Vec3 origin = sensor.GetOrigin();
     const Vec3 direction = sensor.GetDirection();
     const float rayLength = sensor.GetDistance();
     const Vec3 rayCenter =
     origin + direction * (rayLength * 0.5f);

     Mat4 sensorMarkerModel =
     Mat4::Translation(origin.x, origin.y, origin.z) *
     Mat4::Scale(0.15f, 0.15f, 0.15f);
     m_shader.setMat4("model", sensorMarkerModel);
     m_shader.setVec3("color", Vec3(1.0f, 1.0f, 0.0f));
     m_sphere.draw();

     Mat4 rayModel =
     Mat4::Translation(rayCenter.x, rayCenter.y, rayCenter.z) *
     Mat4::RotationY(m_robot.getRotation()) *
     Mat4::Scale(0.035f, 0.035f, rayLength * 0.5f);
     m_shader.setMat4("model", rayModel);

     if (sensor.HasHit()) {
         m_shader.setVec3("color", Vec3(1.0f, 0.0f, 0.0f));
     } else {
         m_shader.setVec3("color", Vec3(0.0f, 1.0f, 0.0f));
     }

     m_cube.draw();
    
    m_window.SwapBuffers();
}

}
