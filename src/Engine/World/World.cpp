#include "Engine/World/World.h"

namespace Engine {

    World::World()
        : m_terrain(4000.0f, 4000.0f, 300),
          m_obstacles{
            // Outer walls
            Obstacle(Vec3(-30, 5, 72.5f), Vec3(1, 5, 77.5f)),
            Obstacle(Vec3(30, 5, 72.5f), Vec3(1, 5, 77.5f)),
            Obstacle(Vec3(0, 5, -5), Vec3(30, 5, 1)),

            // Exit
            Obstacle(Vec3(-19, 5, 150), Vec3(11, 5, 1)),
            Obstacle(Vec3(19, 5, 150), Vec3(11, 5, 1)),

            // Zigzag corridor
            Obstacle(Vec3(-10, 5, 18), Vec3(19, 5, 0.75f)),
            Obstacle(Vec3(10, 5, 38), Vec3(19, 5, 0.75f)),
            Obstacle(Vec3(-10, 5, 58), Vec3(19, 5, 0.75f)),
            Obstacle(Vec3(10, 5, 78), Vec3(19, 5, 0.75f)),
            Obstacle(Vec3(-10, 5, 98), Vec3(19, 5, 0.75f)),
            Obstacle(Vec3(10, 5, 118), Vec3(19, 5, 0.75f)),

            // Obstacle field
            Obstacle(Vec3(0, 3, 28), Vec3(3, 3, 2)),
            Obstacle(Vec3(15, 3, 48), Vec3(2, 3, 4)),
            Obstacle(Vec3(-15, 3, 68), Vec3(2, 3, 3)),

            // Slalom section
            Obstacle(Vec3(-12, 3, 88), Vec3(2, 3, 2)),
            Obstacle(Vec3(8, 3, 92), Vec3(3, 3, 2)),
            Obstacle(Vec3(-5, 3, 108), Vec3(2, 3, 3)),

            // Final challenge
            Obstacle(Vec3(13, 3, 130), Vec3(3, 3, 3)),
            Obstacle(Vec3(-12, 3, 138), Vec3(3, 3, 2))
          }
    {
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
            {
                return true;
            }
        }

        return false;
    }

    const std::vector<Obstacle>& World::getObstacles() const
    {
        return m_obstacles;
    }

}