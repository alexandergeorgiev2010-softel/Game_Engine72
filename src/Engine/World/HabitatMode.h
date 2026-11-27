#pragma once

#include "Engine/Math/Vec3.h"

#include <random>
#include <vector>

namespace Engine {

    class Mesh;
    class PhysicsWorld;
    class Robot;
    class Shader;
    class Window;
    class World;

    class HabitatMode {
    public:
        void reset(Robot& robot, const World& world);
        void step(
            float deltaTime,
            Window& window,
            Robot& robot,
            PhysicsWorld& physicsWorld,
            const World& world
        );
        void draw(const Mesh& cube, const Mesh& sphere, Shader& shader) const;

    private:
        struct Item {
            Vec3 position;
            bool active = true;
        };

        std::vector<Item> m_cells;
        std::vector<Item> m_hazards;
        std::mt19937 m_random{1337};
        Vec3 m_wanderTarget;
        float m_energy = 100.0f;
        float m_respawnTimer = 0.0f;
        float m_statusTimer = 0.0f;
        int m_score = 0;
        bool m_aiEnabled = true;
        bool m_inactive = false;

        void controlAI(float deltaTime, Robot& robot, const World& world);
        void spawnCell(const Robot& robot, const World& world);
        void spawnHazard(const Robot& robot, const World& world);
        Vec3 positionInFront(const Robot& robot, float distance, const World& world) const;
        void updateTitle(Window& window);
    };

}
