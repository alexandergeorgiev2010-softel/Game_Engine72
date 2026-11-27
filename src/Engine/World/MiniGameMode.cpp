#include "Engine/World/MiniGameMode.h"

#include "Engine/Math/Mat4.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/World/Robot.h"
#include "Engine/World/World.h"

#include <cmath>
#include <iostream>

namespace Engine {

    void MiniGameMode::reset(Robot& robot, const World& world)
    {
        const auto atGround = [&world](float x, float z) {
            return Vec3(x, world.GetTerrainHeight(x, z), z);
        };

        m_rewards = {
            {atGround(-18.0f, 15.0f)},
            {atGround(18.0f, 50.0f)},
            {atGround(-18.0f, 85.0f)},
            {atGround(18.0f, 120.0f)},
            {atGround(-18.0f, 155.0f)},
            {atGround(0.0f, 200.0f)}
        };

        m_traps = {
            {atGround(26.0f, 15.0f)},
            {atGround(-40.0f, 50.0f)},
            {atGround(40.0f, 85.0f)},
            {atGround(-40.0f, 120.0f)},
            {atGround(40.0f, 155.0f)},
            {atGround(0.0f, 185.0f)}
        };

        m_score = 0;
        m_finished = false;
        robot.reset(Vec3(0.0f, 0.0f, 0.0f), 0.0f, world);

        std::cout << "Mini-game reset. Collect gold orbs, avoid red traps, reach the goal.\n";
    }

    void MiniGameMode::update(Robot& robot, const World& world)
    {
        if (m_finished) {
            return;
        }

        const Vec3 position = robot.getPosition();
        const float hitRadius = 4.5f * robot.getSize();
        const float hitRadiusSquared = hitRadius * hitRadius;

        for (Item& reward : m_rewards) {
            if (!reward.active) {
                continue;
            }

            const float dx = position.x - reward.position.x;
            const float dz = position.z - reward.position.z;

            if (dx * dx + dz * dz <= hitRadiusSquared) {
                reward.active = false;
                m_score += 10;
                robot.setSize(robot.getSize() + 0.15f);
                std::cout << "Reward collected. Score: " << m_score
                          << " | robot size: " << robot.getSize() << '\n';
            }
        }

        for (Item& trap : m_traps) {
            if (!trap.active) {
                continue;
            }

            const float dx = position.x - trap.position.x;
            const float dz = position.z - trap.position.z;

            if (dx * dx + dz * dz <= hitRadiusSquared) {
                trap.active = false;
                m_score -= 5;
                robot.setSize(robot.getSize() - 0.2f);
                std::cout << "Trap hit. Score: " << m_score
                          << " | robot size: " << robot.getSize() << '\n';
            }
        }

        const Vec3& goal = world.GetGoalPosition();
        const float goalDx = position.x - goal.x;
        const float goalDz = position.z - goal.z;

        if (goalDx * goalDx + goalDz * goalDz <= 12.0f * 12.0f) {
            m_finished = true;
            m_score += 50;
            std::cout << "Course complete! Final score: " << m_score << '\n';
        }
    }

    void MiniGameMode::draw(
        const Mesh& cube,
        const Mesh& sphere,
        Shader& shader
    ) const {
        for (const Item& reward : m_rewards) {
            if (!reward.active) {
                continue;
            }

            const Mat4 model = Mat4::Translation(
                reward.position.x,
                reward.position.y + 2.5f,
                reward.position.z
            ) * Mat4::Scale(1.3f, 1.3f, 1.3f);

            shader.setMat4("model", model);
            shader.setVec3("color", Vec3(1.0f, 0.75f, 0.05f));
            sphere.draw();
        }

        for (const Item& trap : m_traps) {
            if (!trap.active) {
                continue;
            }

            const Mat4 model = Mat4::Translation(
                trap.position.x,
                trap.position.y + 1.0f,
                trap.position.z
            ) * Mat4::Scale(1.8f, 1.0f, 1.8f);

            shader.setMat4("model", model);
            shader.setVec3("color", Vec3(1.0f, 0.05f, 0.05f));
            cube.draw();
        }
    }

    bool MiniGameMode::isFinished() const
    {
        return m_finished;
    }

}
