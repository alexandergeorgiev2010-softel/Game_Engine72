#pragma once

#include "Engine/Controllers/RobotController.h"

#include <array>
#include <cstddef>

namespace Engine {

    class QLearningController : public RobotController {
    public:
        static constexpr std::size_t StateCount = 81;
        static constexpr std::size_t ActionCount = 3;
        using QTable = std::array<std::array<float, ActionCount>, StateCount>;

        QLearningController() = default;
        explicit QLearningController(const QTable& qTable);

        void update(
            float deltaTime,
            Window& window,
            Robot& robot,
            const World& world
        ) override;

        static std::size_t stateFromObservations(
            const std::array<float, 9>& observations
        );

        static std::size_t bestAction(const QTable& qTable, std::size_t state);

    private:
        QTable m_qTable{};
        float m_actionTimeRemaining = 0.0f;
        std::size_t m_currentAction = 0;

        void applyAction(
            std::size_t action,
            float deltaTime,
            Robot& robot,
            const World& world
        ) const;
    };

}
