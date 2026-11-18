#pragma once

#include "Engine/World/Terrain.h"
#include "Engine/World/Obstacle.h"
#include "Engine/Physics/AABB.h"
#include <vector>

namespace Engine {

    class World {
        public:
            World();

            void draw() const;

            bool hasCollision(const AABB& bounds) const;

            const std::vector<Obstacle>& getObstacles() const;

        private:
            Terrain m_terrain;
            std::vector<Obstacle> m_obstacles;
    };

}