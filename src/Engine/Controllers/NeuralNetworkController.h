#pragma once

#include "Engine/Controllers/RobotController.h"

#include <array>

namespace Engine {

    class NeuralNetworkController : public RobotController {
    public:
        static constexpr std::size_t WeightCount = 98;
        using Genome = std::array<float, WeightCount>;

        NeuralNetworkController() = default;
        explicit NeuralNetworkController(const Genome& genome);

        void setGenome(const Genome& genome);
        void update(
            float deltaTime,
            Window& window,
            Robot& robot,
            const World& world
        ) override;

        void act(float deltaTime, Robot& robot, const World& world) const;

    private:
        Genome m_genome{};
        std::array<float, 2> evaluate(const std::array<float, 9>& observations) const;
    };

}
