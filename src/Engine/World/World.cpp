#include "Engine/World/World.h"

#include <algorithm>

namespace Engine {

    World::World()
        : m_terrain(4000.0f, 4000.0f, 300)
    {
        // Places an obstacle directly on the terrain.
        auto addObstacle = [&](float x, float z, const Vec3& scale)
        {
            const float groundHeight = GetTerrainHeight(x, z);
            const float obstacleY = groundHeight + scale.y;

            m_obstacles.emplace_back(Vec3(x, obstacleY, z), scale);
        };

        // Creates a terrain-aligned wall along the X axis.
        auto addWallX = [&](float centerX, float z, float halfLength,
                            float halfHeight, float halfThickness)
        {
            constexpr float segmentLength = 8.0f;

            const float startX = centerX - halfLength;
            const float endX = centerX + halfLength;

            for (float x = startX; x < endX;)
            {
                const float nextX = std::min(x + segmentLength, endX);
                const float segmentCenterX = (x + nextX) * 0.5f;
                const float segmentHalfLength = (nextX - x) * 0.5f;

                addObstacle(segmentCenterX, z, Vec3(segmentHalfLength, halfHeight, halfThickness));
                x = nextX;
            }
        };

        // Creates a terrain-aligned wall along the Z axis.
        auto addWallZ = [&](float x, float centerZ, float halfLength,
                            float halfHeight, float halfThickness)
        {
            constexpr float segmentLength = 8.0f;

            const float startZ = centerZ - halfLength;
            const float endZ = centerZ + halfLength;

            for (float z = startZ; z < endZ;)
            {
                const float nextZ = std::min(z + segmentLength, endZ);
                const float segmentCenterZ = (z + nextZ) * 0.5f;
                const float segmentHalfLength = (nextZ - z) * 0.5f;

                addObstacle(x, segmentCenterZ, Vec3(halfThickness, halfHeight, segmentHalfLength));
                z = nextZ;
            }
        };

        // Creates two wall sections with an opening between them.
        auto addGate = [&](float z, float openingCenterX, float openingWidth)
        {
            constexpr float trackHalfWidth = 50.0f;

            const float openingStart = openingCenterX - openingWidth * 0.5f;
            const float openingEnd = openingCenterX + openingWidth * 0.5f;

            const float leftLength = openingStart + trackHalfWidth;
            const float rightLength = trackHalfWidth - openingEnd;

            if (leftLength > 0.0f)
                addWallX((-trackHalfWidth + openingStart) * 0.5f, z, leftLength * 0.5f, 5, 1);

            if (rightLength > 0.0f)
                addWallX((openingEnd + trackHalfWidth) * 0.5f, z, rightLength * 0.5f, 5, 1);
        };

        // Outer boundaries
        addWallZ(-50, 90, 100, 5, 1);
        addWallZ(50, 90, 100, 5, 1);
        addWallX(0, -10, 50, 5, 1);

        // Alternating gates
        addGate(25, -18, 28);
        addGate(60, 18, 28);
        addGate(95, -18, 28);
        addGate(130, 18, 28);
        addGate(165, -18, 28);

        // Scattered obstacles
        addObstacle(-34, 42, Vec3(3, 3, 3));
        addObstacle(34, 78, Vec3(3, 3, 3));
        addObstacle(-34, 112, Vec3(2, 4, 4));
        addObstacle(34, 148, Vec3(3, 3, 3));

        // Slalom section
        addObstacle(0, 48, Vec3(3, 3, 3));
        addObstacle(0, 82, Vec3(3, 3, 3));
        addObstacle(0, 116, Vec3(3, 3, 3));
        addObstacle(0, 150, Vec3(3, 3, 3));

        // Final section
        addObstacle(-32, 178, Vec3(3, 3, 3));
        addObstacle(32, 178, Vec3(3, 3, 3));

        // Exit
        addWallX(-37, 190, 13, 5, 1);
        addWallX(37, 190, 13, 5, 1);

        // Goal position
        m_goalPosition = Vec3(0.0f, GetTerrainHeight(0.0f, 210.0f), 210.0f);
    }

    void World::draw() const
    {
        m_terrain.draw();
    }

    bool World::hasCollision(const AABB& bounds) const
    {
        for (const Obstacle& obstacle : m_obstacles)
        {
            if (bounds.intersects(obstacle.bounds))
                return true;
        }

        return false;
    }

    const std::vector<Obstacle>& World::getObstacles() const
    {
        return m_obstacles;
    }

    float World::GetTerrainHeight(float x, float z) const
    {
        return m_terrain.GetHeight(x, z);
    }

    const Vec3& World::GetGoalPosition() const
    {
        return m_goalPosition;
    }

}
