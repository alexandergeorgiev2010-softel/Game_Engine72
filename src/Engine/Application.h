#pragma once
#include "Engine/Math/Vec3.h"
#include <string>
#include "Engine/Window/Window.h"
#include "Engine/Physics/Rigidbody.h"
#include "Engine/Physics/PhysicsWorld.h"


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
    void update(float deltaTime, RigidBody& m_RigidBody);
    void render();
    void renderTriangle();
    void renderCube();
    Window m_window;

private:
    ApplicationConfig m_config;
    RigidBody m_RigidBody;
    PhysicsWorld m_PhysicsWorld;
    
    bool m_running = true;
    int m_frame = 0;
};

}
