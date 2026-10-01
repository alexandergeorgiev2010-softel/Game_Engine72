#include "Engine/World/Terrain.h"
#include "Engine/Renderer/Mesh.h"
#include <vector>
#include <cmath>

namespace Engine {
    Terrain::Terrain(float width, float depth, unsigned int resolution) {
        std::vector<float>vertices;
        std::vector<unsigned int>indices;

        float CellWidth = width / resolution;
        float CellDepth = depth / resolution;

        for (unsigned int row = 0; row <= resolution; row ++) {
            for (unsigned int column = 0; column <= resolution; column ++) {
                float x = -width / 2.0f + column * CellWidth;
                float z = -depth / 2.0f + row * CellDepth;
                float y = std::sin(x) * std::cos(z);
                
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
            }
        }

        for (unsigned int row = 0; row < resolution; row ++) {
            for (unsigned int column = 0; column < resolution; column ++) {
                unsigned int topLeft = row * (resolution + 1) + column;
                unsigned int topRight = topLeft + 1;
                unsigned int bottomLeft = (row + 1) * (resolution + 1) + column;
                unsigned int bottomRight = bottomLeft + 1;

                //Triangle 1
                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);

                //Triangle 2
                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);

            }
        }

        m_mesh.SetData(
            vertices.data(),
            vertices.size() / 3,
            indices.data(),
            indices.size()
        );
    }

    void Terrain::draw() const {
        m_mesh.draw();
    }
}