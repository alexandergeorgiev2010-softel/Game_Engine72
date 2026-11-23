#pragma once

#include "Engine/Math/Vec3.h"
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

        float GetTerrainHeight(float x, float z) const;

        const Vec3& GetGoalPosition() const;

    private:
        Terrain m_terrain;
        std::vector<Obstacle> m_obstacles;
        Vec3 m_goalPosition;
    };

}
