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
#include "Engine/World/Robot.h"
#include "Engine/World/MiniGameMode.h"
#include "Engine/World/HabitatMode.h"
#include "Engine/World/BattleArenaMode.h"
#include "Engine/Debug/Profiler.h"
#include "Engine/Sensors/RaySensor.h"
#include "Engine/Controllers/RobotController.h"
#include <memory>
#include <glad/gl.h>


namespace Engine {

struct ApplicationConfig {
    std::string name = "Game Engine 72";
    int maxFrames = 600;
    bool trainQLearning = true;
    bool miniGameMode = false;
    bool habitatMode = false;
    bool battleArenaMode = false;
    bool ruleBasedMode = false;
    float ruleBasedTurnNoiseRadians = 0.15f;
};

class Application {
public:
    explicit Application(ApplicationConfig config);

    void run();

private:
    void update(float deltaTime);
    void render();
    

    Window m_window;
    Mesh m_cube;
    Mesh m_sphere;
    Shader m_shader;

    Robot m_robot;
    MiniGameMode m_miniGame;
    HabitatMode m_habitat;
    BattleArenaMode m_battleArena;

    std::unique_ptr<RobotController> m_controller;

    Camera m_camera;
    World m_world;

    ApplicationConfig m_config;
    PhysicsWorld m_PhysicsWorld;

    Profiler m_profiler;
    
    bool m_running = true;
    int m_frame = 0;
    float m_cubeRotation = 0.0f;

    float m_wTapTimer = 0.0f;
    bool m_WaitingForSecondWTap = false;
};

}
