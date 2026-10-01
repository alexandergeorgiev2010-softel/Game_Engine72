#pragma once

#include "Engine/Renderer/Mesh.h"

namespace Engine {

class Terrain {
    public:
        Terrain(float width, float depth, unsigned int resolution);
        
        void draw() const;
    
    private:
        Mesh m_mesh;
};

}