#pragma once

#include "Engine/Renderer/Mesh.h"
#include "Engine/Procedural/PerlinNoise.h"

namespace Engine {

    class Terrain {
    public:
        Terrain(float width, float depth, unsigned int resolution);

        void draw() const;

        float GetHeight(float x, float z) const;

    private:
        Mesh m_mesh;

        float m_width;
        float m_depth;
        unsigned int m_resolution;

        PerlinNoise m_noise;
    };

}
