#pragma once

#include "Engine/Math/Vec3.h"

#include <vector>

namespace Engine {

    class Mesh;
    class Robot;
    class Shader;
    class World;

    class MiniGameMode {
    public:
        void reset(Robot& robot, const World& world);
        void update(Robot& robot, const World& world);
        void draw(const Mesh& cube, const Mesh& sphere, Shader& shader) const;

        bool isFinished() const;

    private:
        struct Item {
            Vec3 position;
            bool active = true;
        };

        std::vector<Item> m_rewards;
        std::vector<Item> m_traps;
        int m_score = 0;
        bool m_finished = false;
    };

}
