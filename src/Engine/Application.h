#pragma once
#include "Engine/Math/Vec3.h"
#include <string>
#include "Engine/Window/Window.h"
#include "Engine/Physics/Rigidbody.h"
#include "Engine/Physics/PhysicsWorld.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Primitives.h"
#include "Engine/Math/Mat4.h"
#include <glad/gl.h>


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
    

    Window m_window;

    Mesh m_cube;
    Shader m_shader;

    ApplicationConfig m_config;
    RigidBody m_RigidBody;
    PhysicsWorld m_PhysicsWorld;
    
    bool m_running = true;
    int m_frame = 0;
};

}
