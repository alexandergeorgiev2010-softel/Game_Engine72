#pragma once

#include "Engine/World/Terrain.h"

namespace Engine {
    class World {
        public:
            World();
        
            void draw() const;
        
        private:
            Terrain m_terrain;
    };
}