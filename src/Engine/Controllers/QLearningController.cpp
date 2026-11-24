#include "Engine/Controllers/QLearningController.h"
#include "Engine/World/Robot.h"

#include <algorithm>
#include <array>
#include <cmath>

namespace Engine {

    QLearningController::QLearningController(const QTable& qTable)
        : m_qTable(qTable)
    {
    }

    std::size_t QLearningController::stateFromObservations(
        const std::array<float, 9>& observations
    ) {
        const float goalBearing = std::atan2(observations[5], observations[6]);

        std::size_t bearingBin = 1;
        if (goalBearing < -0.3f) {
            bearingBin = 0;
        } else if (goalBearing > 0.3f) {
            bearingBin = 2;
        }

        const float frontDistance =
            std::min({observations[1], observations[2], observations[3]});

        std::size_t frontBin = 2;
        if (frontDistance < 0.3f) {
            frontBin = 0;
        } else if (frontDistance < 0.7f) {
            frontBin = 1;
        }

        const float leftClearance =
            (observations[0] + observations[1]) * 0.5f;
        const float rightClearance =
            (observations[3] + observations[4]) * 0.5f;

        std::size_t openSideBin = 1;
        if (leftClearance > rightClearance + 0.1f) {
            openSideBin = 0;
        } else if (rightClearance > leftClearance + 0.1f) {
            openSideBin = 2;
        }

        std::size_t distanceBin = 0;
        if (observations[7] <= 0.33f) {
            distanceBin = 2;
        } else if (observations[7] <= 0.67f) {
            distanceBin = 1;
        }

        return (((bearingBin * 3 + frontBin) * 3 + openSideBin) * 3 + distanceBin);
    }

    std::size_t QLearningController::bestAction(
        const QTable& qTable,
        std::size_t state
    ) {
        std::size_t action = 0;

        for (std::size_t i = 1; i < ActionCount; ++i) {
            if (qTable[state][i] > qTable[state][action]) {
                action = i;
            }
        }

        return action;
    }

    void QLearningController::applyAction(
        std::size_t action,
        float deltaTime,
        Robot& robot,
        const World& world
    ) const {
        constexpr float turnSpeed = 2.0f;
        constexpr float forwardSpeed = 14.0f;

        if (action == 0) {
            robot.moveForward(forwardSpeed * deltaTime, world);
        } else if (action == 1) {
            robot.rotate(-turnSpeed * deltaTime);
        } else {
            robot.rotate(turnSpeed * deltaTime);
        }
    }

    void QLearningController::update(
        float deltaTime,
        Window& window,
        Robot& robot,
        const World& world
    ) {
        (void)window;

        if (deltaTime <= 0.0f) {
            return;
        }

        if (m_actionTimeRemaining <= 0.0f) {
            const auto observations = robot.getObservations(world);
            const std::size_t state = stateFromObservations(observations);
            m_currentAction = bestAction(m_qTable, state);
            m_actionTimeRemaining = 0.3f;
        }

        applyAction(m_currentAction, deltaTime, robot, world);
        m_actionTimeRemaining -= deltaTime;
    }

}
