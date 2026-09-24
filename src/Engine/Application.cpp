#include "Engine/Application.h"
#include <glad/gl.h>

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
      m_RigidBody({0, 2, 0}, {0, 0, 0}, 1.0f, false),
      m_PhysicsWorld()
{
}

void Application::run()
{
    glEnable(GL_DEPTH_TEST);

    std::cout << "Starting " << m_config.name << '\n';

    Mat4 rotation = Mat4::RotationY(1.5707963f);

Vec4 position(1.0f, 0.0f, 0.0f, 1.0f);

Vec4 result = rotation * position;

std::cout << "Result: ("
          << result.x << ", "
          << result.y << ", "
          << result.z << ", "
          << result.w << ")\n";
     

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
   m_PhysicsWorld.update(deltaTime, m_RigidBody);
}

void Application::render()
{

    std::cout << "Render frame " << m_frame << '\n';

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader.bind();
    m_cube.draw();
    m_window.SwapBuffers();
}

}