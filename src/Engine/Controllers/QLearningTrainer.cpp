#include "Engine/Controllers/QLearningTrainer.h"
#include "Engine/Physics/PhysicsWorld.h"
#include "Engine/World/Robot.h"
#include "Engine/World/World.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <limits>

namespace Engine {

    QLearningTrainer::QLearningTrainer()
        : QLearningTrainer(Config{})
    {
    }

    QLearningTrainer::QLearningTrainer(const Config& config)
        : m_config(config),
          m_random(42)
    {
    }

    std::size_t QLearningTrainer::chooseAction(
        const QLearningController::QTable& qTable,
        std::size_t state,
        float epsilon
    ) {
        std::uniform_real_distribution<float> chance(0.0f, 1.0f);

        if (chance(m_random) < epsilon) {
            std::uniform_int_distribution<std::size_t> randomAction(
                0,
                QLearningController::ActionCount - 1
            );
            return randomAction(m_random);
        }

        return QLearningController::bestAction(qTable, state);
    }

    QLearningController::QTable QLearningTrainer::train(
        Robot& robot,
        PhysicsWorld& physicsWorld,
        const World& world
    ) {
        constexpr float goalRadius = 7.0f;
        constexpr float turnSpeed = 2.0f;
        constexpr float forwardSpeed = 14.0f;

        QLearningController::QTable qTable{};
        const Vec3 startPosition(0.0f, 0.0f, 0.0f);
        const Vec3& goal = world.GetGoalPosition();

        std::size_t successesInBlock = 0;
        float bestDistanceInBlock = std::numeric_limits<float>::infinity();

        for (std::size_t episode = 0; episode < m_config.episodes; ++episode) {
            robot.reset(startPosition, 0.0f, world);

            float epsilon = 1.0f;
            if (m_config.episodes > 1) {
                const float progress = static_cast<float>(episode) /
                                       static_cast<float>(m_config.episodes - 1);
                epsilon = std::max(0.05f, 1.0f - 0.95f * progress);
            }

            bool reachedGoal = false;
            float distance = std::sqrt(
                (goal.x - startPosition.x) * (goal.x - startPosition.x) +
                (goal.z - startPosition.z) * (goal.z - startPosition.z)
            );

            for (std::size_t decision = 0;
                 decision < m_config.maxActionsPerEpisode;
                 ++decision) {
                const auto observations = robot.getObservations(world);
                const std::size_t state =
                    QLearningController::stateFromObservations(observations);
                const std::size_t action = chooseAction(qTable, state, epsilon);
                const Vec3 positionBeforeAction = robot.getPosition();
                const float distanceBeforeAction = distance;

                for (std::size_t frame = 0;
                     frame < m_config.framesPerAction;
                     ++frame) {
                    if (action == 0) {
                        robot.moveForward(forwardSpeed * m_config.deltaTime, world);
                    } else if (action == 1) {
                        robot.rotate(-turnSpeed * m_config.deltaTime);
                    } else {
                        robot.rotate(turnSpeed * m_config.deltaTime);
                    }

                    robot.update(m_config.deltaTime, physicsWorld, world);

                    const Vec3 position = robot.getPosition();
                    const float dx = goal.x - position.x;
                    const float dz = goal.z - position.z;
                    distance = std::sqrt(dx * dx + dz * dz);

                    if (distance <= goalRadius) {
                        reachedGoal = true;
                        break;
                    }
                }

                const Vec3 positionAfterAction = robot.getPosition();
                const float moved = (positionAfterAction - positionBeforeAction).length();
                const float reward =
                    0.25f * (distanceBeforeAction - distance) - 0.03f +
                    (action == 0 && moved < 0.1f ? -1.0f : 0.0f) +
                    (reachedGoal ? 20.0f : 0.0f);

                const auto nextObservations = robot.getObservations(world);
                const std::size_t nextState =
                    QLearningController::stateFromObservations(nextObservations);

                float bestNextValue = qTable[nextState][0];
                for (std::size_t nextAction = 1;
                     nextAction < QLearningController::ActionCount;
                     ++nextAction) {
                    bestNextValue = std::max(bestNextValue, qTable[nextState][nextAction]);
                }

                const float oldValue = qTable[state][action];
                const float target = reachedGoal
                    ? reward
                    : reward + m_config.discount * bestNextValue;

                qTable[state][action] = oldValue +
                    m_config.learningRate * (target - oldValue);

                if (reachedGoal) {
                    break;
                }
            }

            successesInBlock += reachedGoal ? 1u : 0u;
            bestDistanceInBlock = std::min(bestDistanceInBlock, distance);

            if ((episode + 1) % 25 == 0 || episode + 1 == m_config.episodes) {
                std::cout << "Q-learning episodes " << (episode + 1 -
                    ((episode + 1) % 25 == 0 ? 24 : (episode + 1) % 25 - 1))
                    << "-" << (episode + 1)
                    << " | successes: " << successesInBlock
                    << " | closest goal distance: " << bestDistanceInBlock
                    << " | epsilon: " << epsilon << std::endl;

                successesInBlock = 0;
                bestDistanceInBlock = std::numeric_limits<float>::infinity();
            }
        }

        return qTable;
    }

}
