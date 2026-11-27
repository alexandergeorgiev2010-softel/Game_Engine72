#pragma once

#include "Engine/Controllers/RobotController.h"

#include <cstdint>
#include <random>

namespace Engine {

    class RuleBasedController : public RobotController {
    public:
        explicit RuleBasedController(float turnNoiseRadians = 0.15f, std::uint32_t seed = 0);

        void update(
            float deltaTime,
            Window& window,
            Robot& robot,
            const World& world
        ) override;

    private:
        float m_turnNoiseRadians;
        std::mt19937 m_random;
        float m_noiseTimer = 0.0f;
        float m_currentAngleNoise = 0.0f;
    };

}
