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
#include "Engine/Renderer/Camera.h"
#include "Engine/World/World.h"
#include "Engine/World/WorldObject.h"
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
    Mesh m_sphere;
    Shader m_shader;

    WorldObject m_robotTorso;
    WorldObject m_robotHead;
    WorldObject m_robotLeftEye;
    WorldObject m_robotRightEye;
    WorldObject m_robotLeftPupil;
    WorldObject m_robotRightPupil;
    WorldObject m_robotLeftShoulder;
    WorldObject m_robotRightShoulder;
    WorldObject m_robotLeftArm;
    WorldObject m_robotRightArm;
    WorldObject m_robotLeftElbow;
    WorldObject m_robotRightElbow;
    WorldObject m_robotLeftLeg;
    WorldObject m_robotRightLeg;
    Vec3 m_robotPosition{0.0f, 0.0f, 0.0f};

    Camera m_camera;
    World m_world;

    ApplicationConfig m_config;
    RigidBody m_RigidBody;
    PhysicsWorld m_PhysicsWorld;
    
    bool m_running = true;
    int m_frame = 0;
    float m_cubeRotation = 0.0f;

    float m_wTapTimer = 0.0f;
    bool m_WaitingForSecondWTap = false;
};

}
