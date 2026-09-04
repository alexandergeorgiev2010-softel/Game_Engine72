#include "Engine/Application.h"
#include <cmath>
#include <chrono>
#include <iostream>
#include <thread>
#include <utility>

namespace Engine {

Application::Application(ApplicationConfig config)
    : m_config(std::move(config)),
      m_window(1280, 720, "Game Engine 72")
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
        
        update(deltaTime);
        render();

        ++m_frame;
        if (m_frame >= m_config.maxFrames || m_window.ShouldClose()) {
            m_running = false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    std::cout << "Application stopped.\n";
}

void Application::update(float deltaTime)
{
    if (!is_resting) {
        m_velocity += m_gravity * deltaTime;
        m_position += m_velocity * deltaTime;
    }


    if (m_position.y < m_groundHeight) {
        m_position.y = m_groundHeight;
        
        if (std::abs(m_velocity.y) < restThreshold) {
            m_velocity.y = 0.0f;
            is_resting = true;
        }else {
            m_velocity.y = -m_velocity.y * m_restitution;
        }
    }
    m_speed = m_velocity.length();
    
    std::cout << "Speed : " << m_speed << "\n";
    std::cout << "Update frame " << m_frame << " deltaTime=" << deltaTime << "s\n";
    std::cout << "Position: (" << m_position.x << ", " << m_position.y << ", " << m_position.z << ")\n";
    std::cout << "Velocity: (" << m_velocity.x << ", " << m_velocity.y << ", " << m_velocity.z << ")\n";
}

void Application::render()
{
    std::cout << "Render frame " << m_frame << '\n';
}

} 
