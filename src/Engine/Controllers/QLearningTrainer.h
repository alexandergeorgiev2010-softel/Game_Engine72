#pragma once

#include "Engine/Controllers/QLearningController.h"

#include <cstddef>
#include <random>

namespace Engine {

    class Robot;
    class PhysicsWorld;
    class World;

    class QLearningTrainer {
    public:
        struct Config {
            std::size_t episodes = 400;
            std::size_t maxActionsPerEpisode = 150;
            std::size_t framesPerAction = 3;
            float deltaTime = 0.1f;
            float learningRate = 0.15f;
            float discount = 0.95f;
        };

        QLearningTrainer();
        explicit QLearningTrainer(const Config& config);

        QLearningController::QTable train(
            Robot& robot,
            PhysicsWorld& physicsWorld,
            const World& world
        );

    private:
        Config m_config;
        std::mt19937 m_random;

        std::size_t chooseAction(
            const QLearningController::QTable& qTable,
            std::size_t state,
            float epsilon
        );
    };

}
