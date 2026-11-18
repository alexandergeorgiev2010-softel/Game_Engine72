#include "Engine/World/World.h"

namespace Engine {

    World::World()
        : m_terrain(4000.0f, 4000.0f, 300),
          m_obstacles{
              Obstacle(Vec3(0.0f, 1.0f, -10.0f), Vec3(2.0f, 1.0f, 2.0f))
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