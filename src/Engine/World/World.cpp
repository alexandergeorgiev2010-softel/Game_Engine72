#include <Engine/World/World.h>

namespace Engine {
    World::World()
        : m_terrain(4000.0f, 4000.0f, 300)
    {
    }

    void World::draw() const {
        m_terrain.draw();
    }

}
