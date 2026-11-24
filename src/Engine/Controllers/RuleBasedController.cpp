#include "Engine/Controllers/RuleBasedController.h"
#include "Engine/World/Robot.h"

#include <array>
#include <cmath>

namespace Engine {

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

            robot.rotate((leftClearance > rightClearance ? -1.0f : 1.0f) * turnSpeed * deltaTime);
            robot.moveForward(8.0f * deltaTime, world);
            return;
        }

        const float goalBearing =
            std::atan2(observations[5], observations[6]);

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
