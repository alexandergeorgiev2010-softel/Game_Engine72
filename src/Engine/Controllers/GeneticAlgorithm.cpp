#include "Engine/Controllers/GeneticAlgorithm.h"
#include "Engine/Physics/PhysicsWorld.h"
#include "Engine/World/Robot.h"
#include "Engine/World/World.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <numeric>

namespace Engine {

    GeneticAlgorithm::GeneticAlgorithm()
        : GeneticAlgorithm(Config{})
    {
    }

    GeneticAlgorithm::GeneticAlgorithm(const Config& config)
        : m_config(config),
          m_random(42)
    {
    }

    GeneticAlgorithm::Candidate GeneticAlgorithm::evaluate(
        const NeuralNetworkController::Genome& genome,
        Robot& robot,
        PhysicsWorld& physicsWorld,
        const World& world
    ) {
        constexpr float goalRadius = 7.0f;

        robot.reset(Vec3(0.0f, 0.0f, 0.0f), 0.0f, world);

        NeuralNetworkController controller(genome);
        Candidate candidate;
        candidate.genome = genome;

        const Vec3 start = robot.getPosition();
        const Vec3& goal = world.GetGoalPosition();
        const float startDistance = std::sqrt(
            (goal.x - start.x) * (goal.x - start.x) +
            (goal.z - start.z) * (goal.z - start.z)
        );

        float distance = startDistance;
        std::size_t steps = 0;

        for (; steps < m_config.maxStepsPerEpisode; ++steps) {
            controller.act(m_config.deltaTime, robot, world);
            robot.update(m_config.deltaTime, physicsWorld, world);

            const Vec3 position = robot.getPosition();
            const float dx = goal.x - position.x;
            const float dz = goal.z - position.z;
            distance = std::sqrt(dx * dx + dz * dz);

            if (distance <= goalRadius) {
                candidate.reachedGoal = true;
                ++steps;
                break;
            }
        }

        candidate.fitness = startDistance - distance - 0.005f * static_cast<float>(steps);

        if (candidate.reachedGoal) {
            candidate.fitness += 500.0f;
        }

        return candidate;
    }

    const GeneticAlgorithm::Candidate& GeneticAlgorithm::selectParent(
        const std::vector<Candidate>& population
    ) {
        std::uniform_int_distribution<std::size_t> choose(0, population.size() - 1);
        const Candidate* best = &population[choose(m_random)];

        for (int i = 0; i < 2; ++i) {
            const Candidate& candidate = population[choose(m_random)];
            if (candidate.fitness > best->fitness) {
                best = &candidate;
            }
        }

        return *best;
    }

    NeuralNetworkController::Genome GeneticAlgorithm::crossover(
        const NeuralNetworkController::Genome& first,
        const NeuralNetworkController::Genome& second
    ) {
        NeuralNetworkController::Genome child{};
        std::bernoulli_distribution chooseSecond(0.5);

        for (std::size_t i = 0; i < child.size(); ++i) {
            child[i] = chooseSecond(m_random) ? second[i] : first[i];
        }

        return child;
    }

    void GeneticAlgorithm::mutate(NeuralNetworkController::Genome& genome)
    {
        std::bernoulli_distribution shouldMutate(0.08);
        std::normal_distribution<float> mutation(0.0f, 0.25f);

        for (float& weight : genome) {
            if (shouldMutate(m_random)) {
                weight = std::clamp(weight + mutation(m_random), -3.0f, 3.0f);
            }
        }
    }

    NeuralNetworkController::Genome GeneticAlgorithm::train(
        Robot& robot,
        PhysicsWorld& physicsWorld,
        const World& world
    ) {
        using Genome = NeuralNetworkController::Genome;

        std::normal_distribution<float> initialWeight(0.0f, 0.5f);
        std::vector<Candidate> population(m_config.populationSize);

        for (Candidate& candidate : population) {
            for (float& weight : candidate.genome) {
                weight = initialWeight(m_random);
            }
        }

        Genome bestGenome{};
        float bestFitness = -std::numeric_limits<float>::infinity();

        for (std::size_t generation = 0; generation < m_config.generations; ++generation) {
            std::size_t successes = 0;

            for (Candidate& candidate : population) {
                candidate = evaluate(candidate.genome, robot, physicsWorld, world);
                successes += candidate.reachedGoal ? 1u : 0u;
            }

            std::sort(population.begin(), population.end(),
                [](const Candidate& first, const Candidate& second) {
                    return first.fitness > second.fitness;
                });

            if (population.front().fitness > bestFitness) {
                bestFitness = population.front().fitness;
                bestGenome = population.front().genome;
            }

            std::cout << "GA generation " << (generation + 1)
                      << "/" << m_config.generations
                      << " | best fitness: " << population.front().fitness
                      << " | successes: " << successes
                      << "/" << population.size() << std::endl;

            if (generation + 1 == m_config.generations) {
                break;
            }

            std::vector<Candidate> nextPopulation;
            nextPopulation.reserve(population.size());
            nextPopulation.push_back(population[0]);

            if (population.size() > 1) {
                nextPopulation.push_back(population[1]);
            }

            while (nextPopulation.size() < population.size()) {
                const Candidate& first = selectParent(population);
                const Candidate& second = selectParent(population);

                Candidate child;
                child.genome = crossover(first.genome, second.genome);
                mutate(child.genome);
                nextPopulation.push_back(child);
            }

            population = std::move(nextPopulation);
        }

        return bestGenome;
    }

}
