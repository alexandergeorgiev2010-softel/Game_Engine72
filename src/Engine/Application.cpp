#include "Engine/Application.h"
#include <cmath>
#include <chrono>
#include <iostream>
#include <thread>
#include <utility>


namespace Engine {

Application::Application(ApplicationConfig config)
    : m_config(std::move(config)),
      m_window(1600, 900, "Game Engine 72"),
      m_RigidBody({0, 2, 0}, {0, 0, 0}, 1.0f, false),
      m_PhysicsWorld()
{
}

void Application::run()
{
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
        if (m_frame >= m_config.maxFrames || m_window.ShouldClose()) {
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
}

} 
