#pragma once

#include "Engine/Controllers/RobotController.h"

namespace Engine {

    class RuleBasedController : public RobotController {
    public:
        void update(
            float deltaTime,
            Window& window,
            Robot& robot,
            const World& world
        ) override;
    };

}