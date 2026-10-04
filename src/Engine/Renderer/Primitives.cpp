#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Primitives.h"
#include "Engine/Math/Vec3.h"


namespace Engine {

Mesh Primitives::createCube() {
    const float vertices[] = {
        // Front
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,

        // Back
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        // Right
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,

        // Left
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        // Bottom
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        // Top
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f
    };


    const unsigned int indices[] = {
        // Front
        0, 1, 2,
        0, 2, 3,

        // Back
        4, 6, 5,
        4, 7, 6,

        // Right
        8, 9, 10,
        8, 10, 11,

        // Left
        12, 14, 13,
        12, 15, 14,

        // Bottom
        16, 17, 18,
        16, 18, 19,

        // Top
        20, 22, 21,
        20, 23, 22
    };

    Vec3 normals[24];

    for (int i = 0; i < 36; i += 3) {
        unsigned int indexA = indices[i];
        unsigned int indexB = indices[i + 1];
        unsigned int indexC = indices[i + 2];

        Vec3 A (
            vertices[indexA * 3],
            vertices[indexA * 3 + 1],
            vertices[indexA * 3 + 2]
        );

        Vec3 B (
            vertices[indexB * 3],
            vertices[indexB * 3 + 1],
            vertices[indexB * 3 + 2]
        );

        Vec3 C (
            vertices[indexC * 3],
            vertices[indexC * 3 + 1],
            vertices[indexC * 3 + 2]
        );

        Vec3 edge1 = B - A;
        Vec3 edge2 = C - A;

        Vec3 faceNormal = edge1.cross(edge2).normalized();

        normals[indexA] += faceNormal;
        normals[indexB] += faceNormal;
        normals[indexC] += faceNormal;
    }

    float combinedVerticesNormals[24 * 6];

    for (int i = 0; i < 24; i ++) {
        normals[i] = normals[i].normalized();
    }

    for (int i = 0; i < 24; i ++) {
        combinedVerticesNormals[i * 6] = vertices[i * 3];
        combinedVerticesNormals[i * 6 + 1] = vertices[i * 3 + 1];
        combinedVerticesNormals[i * 6 + 2] = (vertices[i * 3 + 2]);

        combinedVerticesNormals[i * 6 + 3] = normals[i].x;
        combinedVerticesNormals[i * 6 + 4] = normals[i].y;
        combinedVerticesNormals[i * 6 + 5] = normals[i].z;
    }

    return Mesh(combinedVerticesNormals, 24, indices, 36);
}

}