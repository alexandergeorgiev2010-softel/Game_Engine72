#pragma once

#include "Engine/Renderer/Mesh.h"
#include "Engine/Procedural/PerlinNoise.h"

#include <vector>

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
        float m_cellWidth;
        float m_cellDepth;

        unsigned int m_resolution;

        std::vector<float> m_heights;

        PerlinNoise m_noise;

        unsigned int getVertexIndex(unsigned int row, unsigned int column) const;
        float GetNoiseHeight(float x, float z) const;
    };

}
