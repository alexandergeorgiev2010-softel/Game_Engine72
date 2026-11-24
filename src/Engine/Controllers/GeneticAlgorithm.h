#pragma once

#include "Engine/Controllers/NeuralNetworkController.h"

#include <cstddef>
#include <random>
#include <vector>

namespace Engine {

    class Robot;
    class PhysicsWorld;
    class World;

    class GeneticAlgorithm {
    public:
        struct Config {
            std::size_t populationSize = 24;
            std::size_t generations = 60;
            std::size_t maxStepsPerEpisode = 400;
            float deltaTime = 0.1f;
        };

        GeneticAlgorithm();
        explicit GeneticAlgorithm(const Config& config);

        NeuralNetworkController::Genome train(
            Robot& robot,
            PhysicsWorld& physicsWorld,
            const World& world
        );

    private:
        struct Candidate {
            NeuralNetworkController::Genome genome{};
            float fitness = 0.0f;
            bool reachedGoal = false;
        };

        Config m_config;
        std::mt19937 m_random;

        Candidate evaluate(
            const NeuralNetworkController::Genome& genome,
            Robot& robot,
            PhysicsWorld& physicsWorld,
            const World& world
        );

        const Candidate& selectParent(const std::vector<Candidate>& population);
        NeuralNetworkController::Genome crossover(
            const NeuralNetworkController::Genome& first,
            const NeuralNetworkController::Genome& second
        );
        void mutate(NeuralNetworkController::Genome& genome);
    };

}
