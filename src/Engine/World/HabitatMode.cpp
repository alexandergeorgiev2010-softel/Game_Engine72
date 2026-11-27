#include "Engine/World/HabitatMode.h"

#include "Engine/Controllers/HumanController.h"
#include "Engine/Math/Mat4.h"
#include "Engine/Physics/PhysicsWorld.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Window/Window.h"
#include "Engine/World/Robot.h"
#include "Engine/World/World.h"

#include <algorithm>
#include <cmath>
#include <sstream>

namespace Engine {

    void HabitatMode::reset(Robot& robot, const World& world)
    {
        m_cells = {
            {Vec3(-18.0f, world.GetTerrainHeight(-18.0f, 12.0f), 12.0f)},
            {Vec3(24.0f, world.GetTerrainHeight(24.0f, 28.0f), 28.0f)},
            {Vec3(-42.0f, world.GetTerrainHeight(-42.0f, -34.0f), -34.0f)}
        };

        m_hazards = {
            {Vec3(0.0f, world.GetTerrainHeight(0.0f, 25.0f), 25.0f)},
            {Vec3(38.0f, world.GetTerrainHeight(38.0f, -8.0f), -8.0f)}
        };

        m_energy = 100.0f;
        m_score = 0;
        m_respawnTimer = 0.0f;
        m_statusTimer = 0.0f;
        m_aiEnabled = true;
        m_inactive = false;
        m_wanderTarget = Vec3(0.0f, 0.0f, 0.0f);
        robot.reset(Vec3(0.0f, 0.0f, 0.0f), 0.0f, world);
    }

    Vec3 HabitatMode::positionInFront(
        const Robot& robot,
        float distance,
        const World& world
    ) const {
        const Vec3 position = robot.getPosition();
        const float rotation = robot.getRotation();
        const float x = std::clamp(position.x + std::sin(rotation) * distance, -65.0f, 65.0f);
        const float z = std::clamp(position.z + std::cos(rotation) * distance, -65.0f, 65.0f);
        return Vec3(x, world.GetTerrainHeight(x, z), z);
    }

    void HabitatMode::spawnCell(const Robot& robot, const World& world)
    {
        m_cells.push_back({positionInFront(robot, 20.0f, world)});
    }

    void HabitatMode::spawnHazard(const Robot& robot, const World& world)
    {
        m_hazards.push_back({positionInFront(robot, 14.0f, world)});
    }

    void HabitatMode::controlAI(float deltaTime, Robot& robot, const World& world)
    {
        const Vec3 position = robot.getPosition();
        Vec3 target = m_wanderTarget;
        float nearestCellDistance = 1000000.0f;

        for (const Item& cell : m_cells) {
            if (!cell.active) {
                continue;
            }

            const float dx = cell.position.x - position.x;
            const float dz = cell.position.z - position.z;
            const float distanceSquared = dx * dx + dz * dz;

            if (distanceSquared < nearestCellDistance) {
                nearestCellDistance = distanceSquared;
                target = cell.position;
            }
        }

        if (nearestCellDistance == 1000000.0f || m_energy > 90.0f) {
            const float dx = m_wanderTarget.x - position.x;
            const float dz = m_wanderTarget.z - position.z;

            if (dx * dx + dz * dz < 12.0f * 12.0f) {
                std::uniform_real_distribution<float> coordinate(-60.0f, 60.0f);
                m_wanderTarget = Vec3(
                    coordinate(m_random),
                    0.0f,
                    coordinate(m_random)
                );
            }

            if (nearestCellDistance == 1000000.0f || m_energy > 90.0f) {
                target = m_wanderTarget;
            }
        }

        Vec3 direction(target.x - position.x, 0.0f, target.z - position.z);

        for (const Item& hazard : m_hazards) {
            if (!hazard.active) {
                continue;
            }

            const float hx = position.x - hazard.position.x;
            const float hz = position.z - hazard.position.z;
            const float distance = std::sqrt(hx * hx + hz * hz);

            if (distance < 20.0f && distance > 0.001f) {
                const float strength = (20.0f - distance) / distance;
                direction.x += hx * strength * 1.5f;
                direction.z += hz * strength * 1.5f;
            }
        }

        const float targetAngle = std::atan2(direction.x, direction.z);
        const float angleDifference = std::atan2(
            std::sin(targetAngle - robot.getRotation()),
            std::cos(targetAngle - robot.getRotation())
        );

        if (std::abs(angleDifference) > 0.2f) {
            robot.rotate((angleDifference > 0.0f ? 1.0f : -1.0f) * 1.8f * deltaTime);
        } else {
            robot.moveForward(10.0f * deltaTime, world);
        }
    }

    void HabitatMode::step(
        float deltaTime,
        Window& window,
        Robot& robot,
        PhysicsWorld& physicsWorld,
        const World& world
    ) {
        if (window.WasKeyPressed(GLFW_KEY_R)) {
            reset(robot, world);
        }

        if (window.WasKeyPressed(GLFW_KEY_H)) {
            m_aiEnabled = !m_aiEnabled;
        }

        if (window.WasKeyPressed(GLFW_KEY_B)) {
            spawnCell(robot, world);
        }

        if (window.WasKeyPressed(GLFW_KEY_T)) {
            spawnHazard(robot, world);
        }

        const bool manualInput =
            window.IsKeyPressed(GLFW_KEY_UP) ||
            window.IsKeyPressed(GLFW_KEY_DOWN) ||
            window.IsKeyPressed(GLFW_KEY_LEFT) ||
            window.IsKeyPressed(GLFW_KEY_RIGHT);

        if (!m_inactive) {
            if (!m_aiEnabled || manualInput) {
                HumanController humanController;
                humanController.update(deltaTime, window, robot, world);
            } else {
                controlAI(deltaTime, robot, world);
            }
        }

        robot.update(deltaTime, physicsWorld, world);

        if (!m_inactive) {
            m_energy = std::max(0.0f, m_energy - deltaTime * 1.0f);
            const Vec3 position = robot.getPosition();
            const float pickupRadius = 5.0f * robot.getSize();
            const float pickupRadiusSquared = pickupRadius * pickupRadius;

            for (Item& cell : m_cells) {
                if (!cell.active) {
                    continue;
                }

                const float dx = position.x - cell.position.x;
                const float dz = position.z - cell.position.z;

                if (dx * dx + dz * dz <= pickupRadiusSquared) {
                    cell.active = false;
                    m_energy = std::min(100.0f, m_energy + 35.0f);
                    m_score += 10;
                    robot.setSize(robot.getSize() + 0.05f);
                }
            }

            for (Item& hazard : m_hazards) {
                if (!hazard.active) {
                    continue;
                }

                const float dx = position.x - hazard.position.x;
                const float dz = position.z - hazard.position.z;

                if (dx * dx + dz * dz <= pickupRadiusSquared) {
                    hazard.active = false;
                    m_energy = std::max(0.0f, m_energy - 25.0f);
                    m_score -= 5;
                    robot.setSize(robot.getSize() - 0.1f);
                }
            }

            if (m_energy <= 0.0f) {
                m_inactive = true;
            }
        }

        bool anyCell = false;
        for (const Item& cell : m_cells) {
            anyCell = anyCell || cell.active;
        }

        if (!anyCell && !m_inactive) {
            m_respawnTimer += deltaTime;
            if (m_respawnTimer >= 5.0f) {
                spawnCell(robot, world);
                m_respawnTimer = 0.0f;
            }
        }

        m_statusTimer += deltaTime;
        if (m_statusTimer >= 0.5f) {
            updateTitle(window);
            m_statusTimer = 0.0f;
        }
    }

    void HabitatMode::updateTitle(Window& window)
    {
        std::ostringstream title;
        title << "Robot Habitat | Energy: " << static_cast<int>(m_energy)
              << " | Score: " << m_score
              << " | " << (m_inactive ? "Offline - R to reset" : (m_aiEnabled ? "AI - arrows assist" : "Manual"));
        window.SetTitle(title.str());
    }

    void HabitatMode::draw(const Mesh& cube, const Mesh& sphere, Shader& shader) const
    {
        for (const Item& cell : m_cells) {
            if (!cell.active) {
                continue;
            }

            const Mat4 model = Mat4::Translation(
                cell.position.x,
                cell.position.y + 2.0f,
                cell.position.z
            ) * Mat4::Scale(1.4f, 1.4f, 1.4f);

            shader.setMat4("model", model);
            shader.setVec3("color", Vec3(0.0f, 0.8f, 1.0f));
            sphere.draw();
        }

        for (const Item& hazard : m_hazards) {
            if (!hazard.active) {
                continue;
            }

            const Mat4 model = Mat4::Translation(
                hazard.position.x,
                hazard.position.y + 1.5f,
                hazard.position.z
            ) * Mat4::Scale(1.4f, 1.5f, 1.4f);

            shader.setMat4("model", model);
            shader.setVec3("color", Vec3(1.0f, 0.05f, 0.05f));
            cube.draw();
        }
    }

}
