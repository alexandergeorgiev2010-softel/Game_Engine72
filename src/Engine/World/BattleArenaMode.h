#pragma once

#include "Engine/Math/Vec3.h"
#include "Engine/World/Robot.h"

#include <cstdint>
#include <memory>
#include <random>
#include <vector>

namespace Engine {

    class Mesh;
    class PhysicsWorld;
    class Shader;
    class Window;

    class BattleArenaMode {
    public:
        void start(const Mesh& cube, const Mesh& sphere, Shader& shader, const World& world);
        void step(float deltaTime, Window& window, PhysicsWorld& physicsWorld, const World& world);
        void draw() const;

    private:
        struct Agent {
            std::unique_ptr<Robot> robot;
            Vec3 color;
            float health = 100.0f;
            float energy = 100.0f;
            float aggression = 0.5f;
            float caution = 0.5f;
            float greed = 0.5f;
            float fireCooldown = 0.0f;
            float score = 0.0f;
            float decisionTimer = 0.0f;
            int id = 0;
            int targetId = -1;
            int behavior = 0;
            bool player = false;
            bool alive = true;
            Vec3 wanderTarget;
            Vec3 targetPosition;
        };

        struct Supply {
            Vec3 position;
            bool active = true;
        };

        struct Projectile {
            Vec3 position;
            Vec3 direction;
            int ownerId = -1;
            float lifetime = 2.0f;
        };

        struct Hazard {
            Vec3 position;
            float lifetime = 20.0f;
        };

        const Mesh* m_cube = nullptr;
        const Mesh* m_sphere = nullptr;
        Shader* m_shader = nullptr;
        std::vector<Agent> m_agents;
        std::vector<Supply> m_supplies;
        std::vector<Projectile> m_projectiles;
        std::vector<Hazard> m_hazards;
        std::mt19937 m_random{std::random_device{}()};
        std::uint32_t m_seed = 0;
        int m_nextId = 0;
        int m_round = 1;
        float m_roundTime = 0.0f;
        float m_titleTimer = 0.0f;
        float m_supplyTimer = 0.0f;
        bool m_roundOver = false;
        bool m_playerAI = false;

        void resetTournament(const World& world);
        void beginNextRound(const World& world);
        void addAgent(bool player, const Vec3& position, float rotation, const World& world);
        Vec3 randomPosition(const World& world);
        void spawnSupply(const Vec3& position, const World& world);
        void fire(Agent& agent, const Vec3& target, bool aimed);
        void updateAgentAI(std::size_t index, float deltaTime, const World& world);
        void updateProjectiles(float deltaTime, const World& world);
        void updateSupplies();
        void updateWindowTitle(Window& window);
        Agent* findPlayer();
        std::size_t livingCount() const;
    };

}
