#pragma once

namespace Engine {

    class Robot;
    class World;
    class Window;

    class RobotController {
    public:
        virtual void update(float deltaTime, Window& window, Robot& robot, const World& world) = 0;

        virtual ~RobotController() = default;
    };

}