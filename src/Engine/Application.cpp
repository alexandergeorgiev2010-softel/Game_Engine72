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
      m_world(),
      m_cube(Primitives::createCube()),
      m_sphere(Primitives::createSphere()),
      m_shader("src/Engine/Renderer/shaders/cube.vert", "src/Engine/Renderer/shaders/cube.frag"),
      m_robotTorso(m_cube, m_shader),
      m_robotHead(m_cube, m_shader),
      m_robotLeftEye(m_sphere, m_shader),
      m_robotRightEye(m_sphere, m_shader),
      m_robotLeftPupil(m_sphere, m_shader),
      m_robotRightPupil(m_sphere, m_shader),
      m_robotLeftShoulder(m_sphere, m_shader),
      m_robotRightShoulder(m_sphere, m_shader),
      m_robotLeftArm(m_cube, m_shader),
      m_robotRightArm(m_cube, m_shader),
      m_robotLeftElbow(m_sphere, m_shader),
      m_robotRightElbow(m_sphere, m_shader),
      m_robotLeftLeg(m_cube, m_shader),
      m_robotRightLeg(m_cube, m_shader),
      m_camera({0.0f, 5.0f, 10.0f}), 
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

   float m_robotSpeed = 9.0f;

   if (m_window.IsKeyPressed(GLFW_KEY_UP)) {
        m_robotPosition.z -= m_robotSpeed * deltaTime; 
   }

   if (m_window.IsKeyPressed(GLFW_KEY_DOWN)) {
        m_robotPosition.z += m_robotSpeed * deltaTime; 
   }

   if (m_window.IsKeyPressed(GLFW_KEY_LEFT)) {
        m_robotPosition.x -= m_robotSpeed * deltaTime; 
   }

   if (m_window.IsKeyPressed(GLFW_KEY_RIGHT)) {
        m_robotPosition.x += m_robotSpeed * deltaTime;
   }

   

   m_PhysicsWorld.update(deltaTime, m_RigidBody);

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


     m_robotTorso.getTransform().position =
     m_robotPosition + Vec3(0.0f, 4.2f, 0.0f);
     m_robotTorso.getTransform().scale = Vec3(2.1, 2.7f, 1.2f);
     m_shader.setVec3("color", Vec3(0.2f, 0.4f, 0.9f));
     m_robotTorso.draw();


     m_robotHead.getTransform().position =
     m_robotPosition + Vec3(0.0f, 7.5f, 0.0f);
     m_robotHead.getTransform().scale = Vec3(1.35f, 1.35f, 1.35f);
     m_shader.setVec3("color", Vec3(0.8f, 0.8f, 0.8f));
     m_robotHead.draw();

     m_robotLeftEye.getTransform().position =
     m_robotPosition + Vec3(-0.4f, 7.7f, 1.4f);
     m_robotLeftEye.getTransform().scale = Vec3(0.25f, 0.25f, 0.25f);
     m_shader.setVec3("color", Vec3(0.1f, 0.9f, 1.0f));
     m_robotLeftEye.draw();

     m_robotRightEye.getTransform().position =
     m_robotPosition + Vec3(0.4f, 7.7f, 1.4f);
     m_robotRightEye.getTransform().scale = Vec3(0.25f, 0.25f, 0.25f);
     m_shader.setVec3("color", Vec3(0.1f, 0.9f, 1.0f));
     m_robotRightEye.draw();

     
     m_robotLeftPupil.getTransform().position =
     m_robotPosition + Vec3(-0.4f, 7.7f, 1.61f);
     m_robotLeftPupil.getTransform().scale = Vec3(0.15f, 0.15f, 0.09f);
     m_shader.setVec3("color", Vec3(0.005f, 0.005f, 0.01f));
     m_robotLeftPupil.draw();

     m_robotRightPupil.getTransform().position =
     m_robotPosition + Vec3(0.4f, 7.7f, 1.61f);
     m_robotRightPupil.getTransform().scale = Vec3(0.15f, 0.15f, 0.09f);
     m_shader.setVec3("color", Vec3(0.005f, 0.005f, 0.01f));
     m_robotRightPupil.draw();


     m_robotLeftShoulder.getTransform().position =
     m_robotPosition + Vec3(-2.15f, 5.65f, 0.0f);
     m_robotLeftShoulder.getTransform().scale = Vec3(0.65f, 0.65f, 0.65f);
     m_shader.setVec3("color", Vec3(0.25f, 0.27f, 0.32f));
     m_robotLeftShoulder.draw();

     m_robotRightShoulder.getTransform().position =
     m_robotPosition + Vec3(2.15f, 5.65f, 0.0f);
     m_robotRightShoulder.getTransform().scale = Vec3(0.65f, 0.65f, 0.65f);
     m_shader.setVec3("color", Vec3(0.25f, 0.27f, 0.32f));
     m_robotRightShoulder.draw();


     m_robotLeftArm.getTransform().position =
     m_robotPosition + Vec3(-2.55f, 4.25f, 0.0f);
     m_robotLeftArm.getTransform().scale = Vec3(0.55f, 1.15f, 0.55f);
     m_shader.setVec3("color", Vec3(0.8f, 0.16f, 0.18f));
     m_robotLeftArm.draw();

     m_robotRightArm.getTransform().position =
     m_robotPosition + Vec3(2.55f, 4.25f, 0.0f);
     m_robotRightArm.getTransform().scale = Vec3(0.55f, 1.15f, 0.55f);
     m_shader.setVec3("color", Vec3(0.8f, 0.16f, 0.18f));
     m_robotRightArm.draw();


     m_robotLeftElbow.getTransform().position =
     m_robotPosition + Vec3(-2.55f, 3.65f, 0.25f);
     m_robotLeftElbow.getTransform().scale = Vec3(0.43f, 0.43f, 0.43f);
     m_shader.setVec3("color", Vec3(0.45f, 0.48f, 0.53f));
     m_robotLeftElbow.draw();

     m_robotRightElbow.getTransform().position =
     m_robotPosition + Vec3(2.55f, 3.65f, 0.25f);
     m_robotRightElbow.getTransform().scale = Vec3(0.43f, 0.43f, 0.43f);
     m_shader.setVec3("color", Vec3(0.45f, 0.48f, 0.53f));
     m_robotRightElbow.draw();


     m_robotLeftLeg.getTransform().position =
     m_robotPosition + Vec3(-0.9f, 1.35f, 0.0f);
     m_robotLeftLeg.getTransform().scale = Vec3(0.75f, 1.5f, 0.9f);
     m_shader.setVec3("color", Vec3(0.2f, 0.2f, 0.25f));
     m_robotLeftLeg.draw();


     m_robotRightLeg.getTransform().position =
     m_robotPosition + Vec3(0.9f, 1.35f, 0.0f);
     m_robotRightLeg.getTransform().scale = Vec3(0.75f, 1.5f, 0.9f);
     m_shader.setVec3("color", Vec3(0.2f, 0.2f, 0.25f));
     m_robotRightLeg.draw();


    float Cube_offset_x = 0;
    float Cube_offset_y = 0;

    Mat4 Cube_Model1[20];
    for (int i = 1; i <= 20; i ++) {
        Cube_offset_x += i * 3.0f;
        Cube_offset_y += i * 3.0f;
        Cube_Model1[i - 1] = Mat4::Translation(Cube_offset_x, Cube_offset_y, 0.0f) * Mat4::RotationY(m_cubeRotation);
        m_shader.setMat4("model", Cube_Model1[i - 1]);
        m_shader.setVec3("color", Vec3(0.8f, 0.2f, 0.2f));
        m_cube.draw();
    }
    
    Mat4 Cube_Model2[20];

    Cube_offset_x = 0.0f;
    Cube_offset_y = 0.0f;

    for (int i = 1; i <= 20; i ++) {
        Cube_offset_x -= i * 3.0f;
        Cube_offset_y += i * 3.0f;

        Cube_Model2[i - 1] = Mat4::Translation(Cube_offset_x, Cube_offset_y, 0.0f) * Mat4::RotationY(m_cubeRotation);
        m_shader.setMat4("model", Cube_Model2[i - 1]);
        m_shader.setVec3("color", Vec3(0.1f, 0.1f, 0.8f));
        m_cube.draw();
    }


    

    
    m_window.SwapBuffers();
}

}