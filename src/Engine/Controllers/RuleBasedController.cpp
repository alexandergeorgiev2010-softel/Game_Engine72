#include "Engine/Controllers/RuleBasedController.h"
#include "Engine/World/Robot.h"

#include <array>
#include <cmath>
#include <algorithm>

namespace Engine {

    RuleBasedController::RuleBasedController(float turnNoiseRadians, std::uint32_t seed)
        : m_turnNoiseRadians(std::max(0.0f, turnNoiseRadians)),
          m_random(seed)
    {
    }

    void RuleBasedController::update(
        float deltaTime,
        Window& window,
        Robot& robot,
        const World& world
    ) {
        (void)window;

        if (deltaTime <= 0.0f) {
            return;
        }

        if (m_noiseTimer <= 0.0f) {
            std::uniform_real_distribution<float> angleNoise(
                -m_turnNoiseRadians,
                m_turnNoiseRadians
            );
            m_currentAngleNoise = angleNoise(m_random);
            m_noiseTimer = 0.25f;
        }
        m_noiseTimer -= deltaTime;

        const std::array<float, 9> observations =
            robot.getObservations(world);

        constexpr float obstacleThreshold = 0.45f;
        constexpr float turnSpeed = 2.0f;

        const bool obstacleAhead =
            observations[1] < obstacleThreshold ||
            observations[2] < obstacleThreshold ||
            observations[3] < obstacleThreshold;

        if (obstacleAhead) {
            const float leftClearance =
                (observations[0] + observations[1]) * 0.5f;

            const float rightClearance =
                (observations[3] + observations[4]) * 0.5f;

            const float direction = leftClearance > rightClearance ? -1.0f : 1.0f;
            const float noiseTurnRate = m_currentAngleNoise / 0.25f;
            robot.rotate((direction * turnSpeed + noiseTurnRate) * deltaTime);
            robot.moveForward(8.0f * deltaTime, world);
            return;
        }

        const float goalBearing =
            std::atan2(observations[5], observations[6]) + m_currentAngleNoise;

        constexpr float headingDeadZone = 0.15f;

        if (goalBearing > headingDeadZone) {
            robot.rotate(turnSpeed * deltaTime);
        } else if (goalBearing < -headingDeadZone) {
            robot.rotate(-turnSpeed * deltaTime);
        } else {
            robot.moveForward(20.0f * deltaTime, world);
        }
    }

}
