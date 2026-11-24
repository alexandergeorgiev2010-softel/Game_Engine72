#include "Engine/Controllers/NeuralNetworkController.h"
#include "Engine/World/Robot.h"

#include <array>
#include <cmath>

namespace Engine {

    NeuralNetworkController::NeuralNetworkController(const Genome& genome)
        : m_genome(genome)
    {
    }

    void NeuralNetworkController::setGenome(const Genome& genome)
    {
        m_genome = genome;
    }

    std::array<float, 2> NeuralNetworkController::evaluate(
        const std::array<float, 9>& observations
    ) const {
        std::array<float, 8> hidden{};

        for (std::size_t node = 0; node < hidden.size(); ++node) {
            float value = m_genome[72 + node];

            for (std::size_t input = 0; input < observations.size(); ++input) {
                value += m_genome[node * 9 + input] * observations[input];
            }

            hidden[node] = std::tanh(value);
        }

        std::array<float, 2> output{};

        for (std::size_t node = 0; node < output.size(); ++node) {
            float value = m_genome[96 + node];

            for (std::size_t input = 0; input < hidden.size(); ++input) {
                value += m_genome[80 + node * 8 + input] * hidden[input];
            }

            output[node] = std::tanh(value);
        }

        return output;
    }

    void NeuralNetworkController::act(
        float deltaTime,
        Robot& robot,
        const World& world
    ) const {
        if (deltaTime <= 0.0f) {
            return;
        }

        const auto observations = robot.getObservations(world);
        const auto action = evaluate(observations);

        robot.rotate(action[0] * 2.0f * deltaTime);
        robot.moveForward(action[1] * 20.0f * deltaTime, world);
    }

    void NeuralNetworkController::update(
        float deltaTime,
        Window& window,
        Robot& robot,
        const World& world
    ) {
        (void)window;
        act(deltaTime, robot, world);
    }

}
