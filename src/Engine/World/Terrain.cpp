#include "Engine/World/Terrain.h"
#include "Engine/Math/Vec3.h"

#include <vector>
#include <cmath>
#include <algorithm>

namespace Engine {

    Terrain::Terrain(float width, float depth, unsigned int resolution)
        : m_width(width),
          m_depth(depth),
          m_cellWidth(width / resolution),
          m_cellDepth(depth / resolution),
          m_resolution(resolution),
          m_noise(1337)
    {
        const unsigned int vertexCount = (resolution + 1) * (resolution + 1);
        const unsigned int indexCount = resolution * resolution * 6;

        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        std::vector<Vec3> normals(vertexCount, Vec3());
        std::vector<float> combinedVerticesNormals;

        vertices.reserve(vertexCount * 3);
        indices.reserve(indexCount);
        combinedVerticesNormals.reserve(vertexCount * 6);
        m_heights.resize(vertexCount);

        for (unsigned int row = 0; row <= resolution; row++) {
            for (unsigned int column = 0; column <= resolution; column++) {
                const float x = -width * 0.5f + column * m_cellWidth;
                const float z = -depth * 0.5f + row * m_cellDepth;

                const float y = GetNoiseHeight(x, z);
                const unsigned int index = getVertexIndex(row, column);

                m_heights[index] = y;

                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
            }
        }

        for (unsigned int row = 0; row < resolution; row++) {
            for (unsigned int column = 0; column < resolution; column++) {
                const unsigned int topLeft = getVertexIndex(row, column);
                const unsigned int topRight = topLeft + 1;
                const unsigned int bottomLeft = getVertexIndex(row + 1, column);
                const unsigned int bottomRight = bottomLeft + 1;

                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);

                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }

        for (unsigned int i = 0; i < indices.size(); i += 3) {
            const unsigned int indexA = indices[i];
            const unsigned int indexB = indices[i + 1];
            const unsigned int indexC = indices[i + 2];

            const Vec3 vertexA(vertices[indexA * 3], vertices[indexA * 3 + 1], vertices[indexA * 3 + 2]);
            const Vec3 vertexB(vertices[indexB * 3], vertices[indexB * 3 + 1], vertices[indexB * 3 + 2]);
            const Vec3 vertexC(vertices[indexC * 3], vertices[indexC * 3 + 1], vertices[indexC * 3 + 2]);

            const Vec3 faceNormal = (vertexB - vertexA).cross(vertexC - vertexA).normalized();

            normals[indexA] += faceNormal;
            normals[indexB] += faceNormal;
            normals[indexC] += faceNormal;
        }

        for (unsigned int i = 0; i < vertexCount; i++) {
            const Vec3 normal = normals[i].normalized();

            combinedVerticesNormals.push_back(vertices[i * 3]);
            combinedVerticesNormals.push_back(vertices[i * 3 + 1]);
            combinedVerticesNormals.push_back(vertices[i * 3 + 2]);

            combinedVerticesNormals.push_back(normal.x);
            combinedVerticesNormals.push_back(normal.y);
            combinedVerticesNormals.push_back(normal.z);
        }

        m_mesh.SetData(
            combinedVerticesNormals.data(),
            vertexCount,
            indices.data(),
            indexCount
        );
    }

    unsigned int Terrain::getVertexIndex(unsigned int row, unsigned int column) const {
        return row * (m_resolution + 1) + column;
    }

    float Terrain::GetNoiseHeight(float x, float z) const {
        return m_noise.GetFractalNoise(
            x * 0.004f,
            z * 0.004f,
            5,
            2.0f,
            0.5f
        ) * 5.0f;
    }

    float Terrain::GetHeight(float x, float z) const {
        const float halfWidth = m_width * 0.5f;
        const float halfDepth = m_depth * 0.5f;

        x = std::clamp(x, -halfWidth, halfWidth);
        z = std::clamp(z, -halfDepth, halfDepth);

        const float gridX = (x + halfWidth) / m_cellWidth;
        const float gridZ = (z + halfDepth) / m_cellDepth;

        const unsigned int column = std::min(
            static_cast<unsigned int>(gridX),
            m_resolution - 1
        );

        const unsigned int row = std::min(
            static_cast<unsigned int>(gridZ),
            m_resolution - 1
        );

        const float localX = gridX - column;
        const float localZ = gridZ - row;

        const float topLeft = m_heights[getVertexIndex(row, column)];
        const float topRight = m_heights[getVertexIndex(row, column + 1)];
        const float bottomLeft = m_heights[getVertexIndex(row + 1, column)];
        const float bottomRight = m_heights[getVertexIndex(row + 1, column + 1)];

        if (localX + localZ <= 1.0f) {
            return topLeft
                + localX * (topRight - topLeft)
                + localZ * (bottomLeft - topLeft);
        }

        return bottomRight
            + (1.0f - localX) * (bottomLeft - bottomRight)
            + (1.0f - localZ) * (topRight - bottomRight);
    }

    void Terrain::draw() const {
        m_mesh.draw();
    }

}
