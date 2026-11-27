#include "Engine/World/BattleArenaMode.h"

#include "Engine/Controllers/HumanController.h"
#include "Engine/Math/Mat4.h"
#include "Engine/Physics/AABB.h"
#include "Engine/Physics/PhysicsWorld.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Window/Window.h"
#include "Engine/World/World.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <sstream>

#include <GLFW/glfw3.h>

namespace Engine {

    namespace {
        constexpr std::size_t startingBotCount = 8;
        constexpr float arenaLimit = 68.0f;
        constexpr float pi = 3.14159265358979323846f;
    }

    void BattleArenaMode::start(
        const Mesh& cube,
        const Mesh& sphere,
        Shader& shader,
        const World& world
    ) {
        m_cube = &cube;
        m_sphere = &sphere;
        m_shader = &shader;

        std::random_device device;
        m_seed = device();
        m_random.seed(m_seed);
        resetTournament(world);

        std::cout << "Battle arena seed: " << m_seed << '\n';
        std::cout << "Controls: arrows move, Space fires, H toggles player AI, B drops supply, T drops hazard, N next round, R restart.\n";
    }

    Vec3 BattleArenaMode::randomPosition(const World& world)
    {
        std::uniform_real_distribution<float> coordinate(-arenaLimit, arenaLimit);

        for (int attempt = 0; attempt < 80; ++attempt) {
            const float x = coordinate(m_random);
            const float z = coordinate(m_random);
            const float y = world.GetTerrainHeight(x, z);
            const AABB bounds(
                Vec3(x - 4.0f, y, z - 4.0f),
                Vec3(x + 4.0f, y + 9.0f, z + 4.0f)
            );

            if (world.hasCollision(bounds)) {
                continue;
            }

            bool tooClose = false;
            for (const Agent& agent : m_agents) {
                if (!agent.alive || !agent.robot) {
                    continue;
                }

                const Vec3 other = agent.robot->getPosition();
                const float dx = x - other.x;
                const float dz = z - other.z;
                if (dx * dx + dz * dz < 18.0f * 18.0f) {
                    tooClose = true;
                    break;
                }
            }

            if (!tooClose) {
                return Vec3(x, y, z);
            }
        }

        const float x = coordinate(m_random) * 0.7f;
        const float z = coordinate(m_random) * 0.7f;
        return Vec3(x, world.GetTerrainHeight(x, z), z);
    }

    void BattleArenaMode::addAgent(
        bool player,
        const Vec3& position,
        float rotation,
        const World& world
    ) {
        static const std::array<Vec3, 8> colors = {
            Vec3(0.15f, 0.55f, 1.0f),
            Vec3(1.0f, 0.25f, 0.2f),
            Vec3(0.8f, 0.25f, 1.0f),
            Vec3(1.0f, 0.65f, 0.1f),
            Vec3(0.2f, 0.9f, 0.35f),
            Vec3(0.1f, 0.85f, 0.9f),
            Vec3(0.95f, 0.4f, 0.65f),
            Vec3(0.75f, 0.85f, 0.2f)
        };

        std::uniform_real_distribution<float> trait(0.0f, 1.0f);
        Agent agent;
        agent.robot = std::make_unique<Robot>(*m_cube, *m_sphere, *m_shader, position);
        agent.robot->reset(position, rotation, world);
        agent.color = player ? colors[0] : colors[1 + (m_nextId % (colors.size() - 1))];
        agent.health = 100.0f;
        agent.energy = 100.0f;
        agent.aggression = trait(m_random);
        agent.caution = trait(m_random);
        agent.greed = trait(m_random);
        agent.id = m_nextId++;
        agent.player = player;
        agent.alive = true;
        agent.wanderTarget = position;
        agent.targetPosition = position;
        m_agents.push_back(std::move(agent));
    }

    void BattleArenaMode::spawnSupply(const Vec3& position, const World& world)
    {
        m_supplies.push_back({Vec3(
            position.x,
            world.GetTerrainHeight(position.x, position.z),
            position.z
        )});
    }

    void BattleArenaMode::resetTournament(const World& world)
    {
        m_agents.clear();
        m_supplies.clear();
        m_projectiles.clear();
        m_hazards.clear();
        m_round = 1;
        m_nextId = 0;
        m_roundTime = 0.0f;
        m_supplyTimer = 0.0f;
        m_roundOver = false;
        m_playerAI = false;

        addAgent(true, Vec3(0.0f, 0.0f, 0.0f), 0.0f, world);

        std::uniform_real_distribution<float> angle(-pi, pi);
        while (m_agents.size() < startingBotCount) {
            addAgent(false, randomPosition(world), angle(m_random), world);
        }

        for (int i = 0; i < 12; ++i) {
            spawnSupply(randomPosition(world), world);
        }
    }

    void BattleArenaMode::beginNextRound(const World& world)
    {
        std::vector<Agent> survivors;
        survivors.reserve(startingBotCount);

        for (Agent& agent : m_agents) {
            if (agent.alive) {
                survivors.push_back(std::move(agent));
            }
        }

        m_agents = std::move(survivors);

        if (m_agents.empty()) {
            resetTournament(world);
            return;
        }

        bool hasPlayer = false;
        for (Agent& agent : m_agents) {
            hasPlayer = hasPlayer || agent.player;
            agent.health = 100.0f;
            agent.energy = 100.0f;
            agent.fireCooldown = 0.0f;
            agent.decisionTimer = 0.0f;
            agent.targetId = -1;
            agent.behavior = 0;
            agent.alive = true;
        }

        if (!hasPlayer) {
            m_agents.front().player = true;
        }

        m_round++;
        m_roundTime = 0.0f;
        m_supplyTimer = 0.0f;
        m_roundOver = false;
        m_playerAI = false;
        m_projectiles.clear();
        m_supplies.clear();
        m_hazards.clear();

        std::uniform_real_distribution<float> angle(-pi, pi);
        for (Agent& agent : m_agents) {
            const Vec3 position = randomPosition(world);
            agent.robot->reset(position, angle(m_random), world);
        }

        while (m_agents.size() < startingBotCount) {
            addAgent(!hasPlayer, randomPosition(world), angle(m_random), world);
            hasPlayer = hasPlayer || m_agents.back().player;
        }

        for (int i = 0; i < 12; ++i) {
            spawnSupply(randomPosition(world), world);
        }

        std::cout << "Round " << m_round << " started. " << m_agents.size()
                  << " robots remain or enter.\n";
    }

    BattleArenaMode::Agent* BattleArenaMode::findPlayer()
    {
        for (Agent& agent : m_agents) {
            if (agent.player && agent.alive) {
                return &agent;
            }
        }
        return nullptr;
    }

    std::size_t BattleArenaMode::livingCount() const
    {
        return static_cast<std::size_t>(std::count_if(
            m_agents.begin(), m_agents.end(),
            [](const Agent& agent) { return agent.alive; }
        ));
    }

    void BattleArenaMode::fire(Agent& agent, const Vec3& target, bool aimed)
    {
        if (agent.fireCooldown > 0.0f || agent.energy < 8.0f) {
            return;
        }

        const Vec3 position = agent.robot->getPosition();
        const float dx = target.x - position.x;
        const float dz = target.z - position.z;
        float angle = std::atan2(dx, dz);

        if (aimed) {
            std::uniform_real_distribution<float> spread(-0.14f, 0.14f);
            angle += spread(m_random);
        }

        const Vec3 direction(std::sin(angle), 0.0f, std::cos(angle));
        const float size = agent.robot->getSize();
        m_projectiles.push_back({
            position + direction * (4.0f * size) + Vec3(0.0f, 4.0f * size, 0.0f),
            direction,
            agent.id,
            2.5f
        });

        agent.energy -= 8.0f;
        agent.fireCooldown = 0.8f;
    }

    void BattleArenaMode::updateAgentAI(
        std::size_t index,
        float deltaTime,
        const World& world
    ) {
        Agent& agent = m_agents[index];
        const Vec3 position = agent.robot->getPosition();

        if (agent.decisionTimer <= 0.0f) {
            std::uniform_real_distribution<float> chance(0.0f, 1.0f);
            std::uniform_real_distribution<float> coordinate(-arenaLimit, arenaLimit);

            int nearestSupply = -1;
            float nearestSupplyDistance = 1000000.0f;
            for (std::size_t i = 0; i < m_supplies.size(); ++i) {
                if (!m_supplies[i].active) {
                    continue;
                }
                const float dx = m_supplies[i].position.x - position.x;
                const float dz = m_supplies[i].position.z - position.z;
                const float distance = dx * dx + dz * dz;
                if (distance < nearestSupplyDistance) {
                    nearestSupplyDistance = distance;
                    nearestSupply = static_cast<int>(i);
                }
            }

            int nearestEnemy = -1;
            float nearestEnemyDistance = 1000000.0f;
            for (std::size_t i = 0; i < m_agents.size(); ++i) {
                if (i == index || !m_agents[i].alive) {
                    continue;
                }
                const Vec3 enemy = m_agents[i].robot->getPosition();
                const float dx = enemy.x - position.x;
                const float dz = enemy.z - position.z;
                const float distance = dx * dx + dz * dz;
                if (distance < nearestEnemyDistance) {
                    nearestEnemyDistance = distance;
                    nearestEnemy = static_cast<int>(i);
                }
            }

            agent.behavior = 0;
            agent.targetId = -1;

            const float randomChoice = chance(m_random);
            if (nearestEnemy >= 0 && nearestEnemyDistance < 38.0f * 38.0f &&
                randomChoice < agent.aggression * 0.75f) {
                agent.behavior = 1;
                agent.targetId = m_agents[static_cast<std::size_t>(nearestEnemy)].id;
            } else if (nearestSupply >= 0 &&
                       (agent.energy < 72.0f || randomChoice < agent.greed * 0.65f)) {
                agent.behavior = 2;
                agent.targetPosition = m_supplies[static_cast<std::size_t>(nearestSupply)].position;
            } else if (nearestEnemy >= 0 && agent.health < 55.0f &&
                       randomChoice < agent.caution) {
                const Vec3 enemy = m_agents[static_cast<std::size_t>(nearestEnemy)].robot->getPosition();
                Vec3 away(position.x - enemy.x, 0.0f, position.z - enemy.z);
                away = away.normalized();
                agent.behavior = 3;
                agent.targetPosition = Vec3(
                    std::clamp(position.x + away.x * 35.0f, -arenaLimit, arenaLimit),
                    0.0f,
                    std::clamp(position.z + away.z * 35.0f, -arenaLimit, arenaLimit)
                );
            } else {
                agent.wanderTarget = Vec3(coordinate(m_random), 0.0f, coordinate(m_random));
                agent.targetPosition = agent.wanderTarget;
            }

            agent.decisionTimer = 0.45f + chance(m_random) * 0.55f;
        }

        agent.decisionTimer -= deltaTime;
        Vec3 target = agent.targetPosition;

        if (agent.behavior == 1) {
            for (const Agent& other : m_agents) {
                if (other.id == agent.targetId && other.alive) {
                    target = other.robot->getPosition();
                    const float dx = target.x - position.x;
                    const float dz = target.z - position.z;
                    if (dx * dx + dz * dz < 42.0f * 42.0f && agent.aggression > 0.35f) {
                        std::uniform_real_distribution<float> chance(0.0f, 1.0f);
                        if (chance(m_random) < agent.aggression * deltaTime * 2.0f) {
                            fire(agent, target, true);
                        }
                    }
                    break;
                }
            }
        }

        Vec3 direction(target.x - position.x, 0.0f, target.z - position.z);

        for (const Hazard& hazard : m_hazards) {
            const float hx = position.x - hazard.position.x;
            const float hz = position.z - hazard.position.z;
            const float distance = std::sqrt(hx * hx + hz * hz);
            if (distance < 18.0f && distance > 0.001f) {
                const float strength = (18.0f - distance) / distance;
                direction.x += hx * strength * 1.8f;
                direction.z += hz * strength * 1.8f;
            }
        }

        const auto& sensors = agent.robot->getRaySensors();
        if (sensors[2].GetDistance() < 8.0f) {
            const float left = sensors[0].GetDistance() + sensors[1].GetDistance();
            const float right = sensors[3].GetDistance() + sensors[4].GetDistance();
            agent.robot->rotate((left > right ? -1.0f : 1.0f) * 2.0f * deltaTime);
            return;
        }

        const float targetAngle = std::atan2(direction.x, direction.z);
        const float difference = std::atan2(
            std::sin(targetAngle - agent.robot->getRotation()),
            std::cos(targetAngle - agent.robot->getRotation())
        );

        if (std::abs(difference) > 0.25f) {
            const float turn = std::clamp(difference, -1.8f * deltaTime, 1.8f * deltaTime);
            agent.robot->rotate(turn);
        } else {
            agent.robot->moveForward(12.0f * deltaTime, world);
        }
    }

    void BattleArenaMode::updateProjectiles(float deltaTime, const World& world)
    {
        for (auto projectile = m_projectiles.begin(); projectile != m_projectiles.end();) {
            projectile->position += projectile->direction * (42.0f * deltaTime);
            projectile->lifetime -= deltaTime;

            const Vec3 p = projectile->position;
            const AABB bounds(p - Vec3(0.6f, 0.6f, 0.6f), p + Vec3(0.6f, 0.6f, 0.6f));
            bool remove = projectile->lifetime <= 0.0f || world.hasCollision(bounds);

            if (!remove) {
                for (Agent& target : m_agents) {
                    if (!target.alive || target.id == projectile->ownerId) {
                        continue;
                    }

                    const Vec3 targetPosition = target.robot->getPosition();
                    const float dx = p.x - targetPosition.x;
                    const float dy = p.y - (targetPosition.y + 4.0f * target.robot->getSize());
                    const float dz = p.z - targetPosition.z;

                    if (dx * dx + dy * dy + dz * dz < 4.0f * 4.0f) {
                        target.health -= 25.0f;
                        remove = true;

                        if (target.health <= 0.0f) {
                            target.alive = false;
                            for (Agent& owner : m_agents) {
                                if (owner.id == projectile->ownerId) {
                                    owner.score += 1.0f;
                                    break;
                                }
                            }
                            std::cout << "Robot " << target.id << " eliminated.\n";
                        }
                        break;
                    }
                }
            }

            if (remove) {
                projectile = m_projectiles.erase(projectile);
            } else {
                ++projectile;
            }
        }
    }

    void BattleArenaMode::updateSupplies()
    {
        for (Agent& agent : m_agents) {
            if (!agent.alive) {
                continue;
            }

            const Vec3 position = agent.robot->getPosition();
            const float radius = 5.0f * agent.robot->getSize();

            for (Supply& supply : m_supplies) {
                if (!supply.active) {
                    continue;
                }

                const float dx = position.x - supply.position.x;
                const float dz = position.z - supply.position.z;

                if (dx * dx + dz * dz <= radius * radius) {
                    supply.active = false;
                    agent.energy = std::min(100.0f, agent.energy + 35.0f);
                    agent.health = std::min(100.0f, agent.health + 10.0f);
                    agent.score += 10.0f;
                    agent.robot->setSize(agent.robot->getSize() + 0.04f);
                }
            }
        }

    }

    void BattleArenaMode::updateWindowTitle(Window& window)
    {
        std::ostringstream title;
        title << "Robot Arena | Round " << m_round
              << " | Alive: " << livingCount();

        const Agent* player = nullptr;
        for (const Agent& agent : m_agents) {
            if (agent.player) {
                player = &agent;
                break;
            }
        }

        if (player) {
            title << " | HP " << static_cast<int>(std::max(0.0f, player->health))
                  << " | Energy " << static_cast<int>(player->energy)
                  << " | Score " << static_cast<int>(player->score)
                  << (m_playerAI ? " | Player AI" : " | Manual");
        }

        if (m_roundOver) {
            title << " | Round over - N next, R restart";
        }

        window.SetTitle(title.str());
    }

    void BattleArenaMode::step(
        float deltaTime,
        Window& window,
        PhysicsWorld& physicsWorld,
        const World& world
    ) {
        if (window.WasKeyPressed(GLFW_KEY_R)) {
            std::random_device device;
            m_seed = device();
            m_random.seed(m_seed);
            resetTournament(world);
        }

        if (m_roundOver) {
            if (window.WasKeyPressed(GLFW_KEY_N)) {
                beginNextRound(world);
            }
            updateWindowTitle(window);
            return;
        }

        if (window.WasKeyPressed(GLFW_KEY_H)) {
            m_playerAI = !m_playerAI;
        }

        Agent* player = findPlayer();

        if (player && window.WasKeyPressed(GLFW_KEY_B)) {
            const float angle = player->robot->getRotation();
            const Vec3 p = player->robot->getPosition();
            const float x = std::clamp(p.x + std::sin(angle) * 18.0f, -arenaLimit, arenaLimit);
            const float z = std::clamp(p.z + std::cos(angle) * 18.0f, -arenaLimit, arenaLimit);
            spawnSupply(Vec3(x, 0.0f, z), world);
        }

        if (player && window.WasKeyPressed(GLFW_KEY_T)) {
            const float angle = player->robot->getRotation();
            const Vec3 p = player->robot->getPosition();
            const float x = std::clamp(p.x + std::sin(angle) * 14.0f, -arenaLimit, arenaLimit);
            const float z = std::clamp(p.z + std::cos(angle) * 14.0f, -arenaLimit, arenaLimit);
            m_hazards.push_back({Vec3(x, world.GetTerrainHeight(x, z), z), 18.0f});
        }

        m_roundTime += deltaTime;

        for (std::size_t i = 0; i < m_agents.size(); ++i) {
            Agent& agent = m_agents[i];
            if (!agent.alive) {
                continue;
            }

            agent.fireCooldown = std::max(0.0f, agent.fireCooldown - deltaTime);
            agent.energy = std::max(0.0f, agent.energy - deltaTime * 0.12f);

            if (agent.energy <= 0.0f) {
                agent.health -= deltaTime * 5.0f;
            }

            if (agent.player && !m_playerAI) {
                HumanController human;
                human.update(deltaTime, window, *agent.robot, world);

                if (window.IsKeyPressed(GLFW_KEY_SPACE)) {
                    const Vec3 p = agent.robot->getPosition();
                    const float angle = agent.robot->getRotation();
                    fire(agent, p + Vec3(std::sin(angle) * 40.0f, 0.0f, std::cos(angle) * 40.0f), false);
                }
            } else {
                updateAgentAI(i, deltaTime, world);
            }

            agent.robot->update(deltaTime, physicsWorld, world);

            for (const Hazard& hazard : m_hazards) {
                const Vec3 p = agent.robot->getPosition();
                const float dx = p.x - hazard.position.x;
                const float dz = p.z - hazard.position.z;
                if (dx * dx + dz * dz < 5.0f * 5.0f) {
                    agent.health -= 20.0f * deltaTime;
                }
            }

            if (agent.health <= 0.0f) {
                agent.alive = false;
                std::cout << "Robot " << agent.id << " eliminated.\n";
            }
        }

        updateProjectiles(deltaTime, world);
        updateSupplies();

        for (Hazard& hazard : m_hazards) {
            hazard.lifetime -= deltaTime;
        }
        m_hazards.erase(
            std::remove_if(m_hazards.begin(), m_hazards.end(),
                [](const Hazard& hazard) { return hazard.lifetime <= 0.0f; }),
            m_hazards.end()
        );

        std::size_t activeSupplies = 0;
        for (const Supply& supply : m_supplies) {
            activeSupplies += supply.active ? 1u : 0u;
        }
        m_supplyTimer += deltaTime;
        if (activeSupplies < 8 && m_supplyTimer >= 3.0f) {
            spawnSupply(randomPosition(world), world);
            m_supplyTimer = 0.0f;
        }

        if (livingCount() <= 1 || m_roundTime >= 90.0f) {
            m_roundOver = true;
            std::cout << "Round " << m_round << " over. " << livingCount()
                      << " survivor(s). Press N for the next round or R to restart.\n";
        }

        m_titleTimer += deltaTime;
        if (m_titleTimer >= 0.25f) {
            updateWindowTitle(window);
            m_titleTimer = 0.0f;
        }
    }

    void BattleArenaMode::draw() const
    {
        if (!m_shader || !m_cube || !m_sphere) {
            return;
        }

        for (const Agent& agent : m_agents) {
            if (agent.alive) {
                agent.robot->draw(agent.color);
            }
        }

        for (const Supply& supply : m_supplies) {
            if (!supply.active) {
                continue;
            }

            const Mat4 model = Mat4::Translation(
                supply.position.x,
                supply.position.y + 2.0f,
                supply.position.z
            ) * Mat4::Scale(1.2f, 1.2f, 1.2f);

            m_shader->setMat4("model", model);
            m_shader->setVec3("color", Vec3(0.0f, 0.85f, 1.0f));
            m_sphere->draw();
        }

        for (const Hazard& hazard : m_hazards) {
            const Mat4 model = Mat4::Translation(
                hazard.position.x,
                hazard.position.y + 0.8f,
                hazard.position.z
            ) * Mat4::Scale(2.0f, 0.8f, 2.0f);

            m_shader->setMat4("model", model);
            m_shader->setVec3("color", Vec3(1.0f, 0.08f, 0.03f));
            m_cube->draw();
        }

        for (const Projectile& projectile : m_projectiles) {
            const Mat4 model = Mat4::Translation(
                projectile.position.x,
                projectile.position.y,
                projectile.position.z
            ) * Mat4::Scale(0.45f, 0.45f, 0.45f);

            m_shader->setMat4("model", model);
            m_shader->setVec3("color", Vec3(1.0f, 0.9f, 0.15f));
            m_sphere->draw();
        }
    }

}
