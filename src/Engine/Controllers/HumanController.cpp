#include "Engine/Controllers/HumanController.h"
#include "Engine/World/Robot.h"
#include "Engine/Window/Window.h"

#include <GLFW/glfw3.h>

namespace Engine {

void HumanController::update(float deltaTime, Window& window, Robot& robot, const World& world)
{
    float robotSpeed = 20.0f;

    if (window.IsKeyPressed(GLFW_KEY_UP)) {
        robot.moveForward(robotSpeed * deltaTime, world);
    }

    if (window.IsKeyPressed(GLFW_KEY_DOWN)) {
        robot.moveBackward(robotSpeed * deltaTime, world);
    }

    if (window.IsKeyPressed(GLFW_KEY_LEFT)) {
        robot.rotate(2.0f * deltaTime);
    }

    if (window.IsKeyPressed(GLFW_KEY_RIGHT)) {
        robot.rotate(-2.0f * deltaTime);
    }
}

}
