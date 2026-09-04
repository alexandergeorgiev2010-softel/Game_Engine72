#pragma once
#include "Engine/Math/Vec3.h"
#include <string>
#include "Engine/Window/Window.h"

namespace Engine {

struct ApplicationConfig {
    std::string name = "Game Engine 72";
    int maxFrames = 600;
};

class Application {
public:
    explicit Application(ApplicationConfig config);

    void run();

private:
    void update(float deltaTime);
    void render();
    Window m_window;

private:
    ApplicationConfig m_config;
    bool m_running = true;
    int m_frame = 0;
    Vec3 m_position = {0.0f, 0.0f, 0.0f};
    Vec3 m_velocity = {0.0f, 2.0f, 0.0f};
    Vec3 m_gravity = {0.0f, -9.81f, 0.0f};
    float m_speed = 0.0f;
    const float m_groundHeight = 0.0f;
    const float m_restitution = 0.7f;
    const float restThreshold = 0.05f;
    bool is_resting = false;
};

}
