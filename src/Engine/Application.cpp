#include "Engine/Application.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>
#include <thread>
#include <utility>



namespace Engine {

Application::Application(ApplicationConfig config)
    : m_config(std::move(config)),
      m_window(1600, 900, "Game Engine 72"),
      m_cube(Primitives::createCube()),
      m_shader("src/Engine/Renderer/shaders/cube.vert", "src/Engine/Renderer/shaders/cube.frag"),
      m_camera({0.0f, 0.0f, 5.0f}), 
      m_RigidBody({0, 2, 0}, {0, 0, 0}, 1.0f, false),
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
        m_window.PollEvents();

        const auto currentFrameTime = std::chrono::steady_clock::now();
        const std::chrono::duration<float> frameDuration = currentFrameTime - lastFrameTime;
        const float deltaTime = frameDuration.count();
        lastFrameTime = currentFrameTime;

        update(deltaTime, m_RigidBody);
        render();

        ++m_frame;
        if (/*m_frame >= m_config.maxFrames ||*/ m_window.ShouldClose()) {
            m_running = false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    std::cout << "Application stopped.\n";
}

void Application::update(float deltaTime, RigidBody& m_RigidBody)
{
   float speed = 5.0f;

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

   if (m_window.WasKeyPressed(GLFW_KEY_F1)) {
        m_window.ToggleMouseCapture();
   }

   m_PhysicsWorld.update(deltaTime, m_RigidBody);

   if (m_window.IsMouseCaptured()) {
        Vec3 MouseDelta = m_window.GetMouseDelta();
        float sensitivity = 0.002f;

        m_camera.Rotate(MouseDelta.x * sensitivity, MouseDelta.y * sensitivity);
   }
   m_cubeRotation += 1.0f * deltaTime;
  
}

void Application::render()
{

    std::cout << "Render frame " << m_frame << '\n';

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader.bind();

    Mat4 model = Mat4::RotationY(m_cubeRotation);
    m_shader.setMat4("model", model);

    Mat4 view = m_camera.getViewMatrix();
    m_shader.setMat4("view", view);

    int width;
    int height;

    m_window.GetWindowSize(width, height);

    float aspect = static_cast<float>(width) / static_cast<float>(height);

    Mat4 projection = Mat4::Perspective(70.0f * 3.14159265 / 180.0f, aspect, 0.1f, 100.0f);
    m_shader.setMat4("projection", projection);

    m_cube.draw();
    m_window.SwapBuffers();
}

}