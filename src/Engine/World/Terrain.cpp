#include "Engine/World/Terrain.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Math/Vec3.h"
#include <vector>
#include <cmath>

namespace Engine {
    Terrain::Terrain(float width, float depth, unsigned int resolution) {
        std::vector<float>vertices;
        std::vector<unsigned int>indices;
        std::vector<Vec3>normals;
        std::vector<float>combinedVerticesNormals;

        normals.resize((resolution + 1) * (resolution + 1), Vec3(0.0f, 0.0f, 0.0f));


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

        for (unsigned int i = 0; i < indices.size(); i += 3) {
            unsigned indexA = indices[i];
            unsigned int indexB = indices[i + 1];
            unsigned int indexC = indices[i + 2];

            Vec3 vertexA(
                vertices[indexA * 3],
                vertices[indexA * 3 + 1],
                vertices[indexA * 3 + 2]
            );

            Vec3 vertexB(
                vertices[indexB * 3],
                vertices[indexB * 3 + 1],
                vertices[indexB * 3 + 2]
            );

            Vec3 vertexC(
                vertices[indexC * 3],
                vertices[indexC * 3 + 1],
                vertices[indexC * 3 + 2]
            );

            Vec3 edge1 = vertexB - vertexA;
            Vec3 edge2 = vertexC - vertexA;

            Vec3 faceNormal = edge1.cross(edge2).normalized();

            normals[indexA] += faceNormal;
            normals[indexB] += faceNormal;
            normals[indexC] += faceNormal;
        }

        for (Vec3& normal: normals) {
            normal = normal.normalized();
        }

        for (unsigned int i = 0; i < vertices.size() / 3; i ++) {
            combinedVerticesNormals.push_back(vertices[i * 3]);
            combinedVerticesNormals.push_back(vertices[i * 3 + 1]);
            combinedVerticesNormals.push_back(vertices[i * 3 + 2]);

            combinedVerticesNormals.push_back(normals[i].x);
            combinedVerticesNormals.push_back(normals[i].y);
            combinedVerticesNormals.push_back(normals[i].z);
        }

       
        m_mesh.SetData(
            combinedVerticesNormals.data(),
            combinedVerticesNormals.size() / 6,
            indices.data(),
            indices.size()
        );
    }

    void Terrain::draw() const {
        m_mesh.draw();
    }
}