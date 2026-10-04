#include "Engine/Renderer/Mesh.h"

namespace Engine {

Mesh::Mesh()
{
    
}

Mesh::Mesh(const float* combinedVerticesNormals, unsigned int combinedVerticesNormalsCount, const unsigned int* indices, unsigned int indicesCount)
{
    SetData(combinedVerticesNormals, combinedVerticesNormalsCount, indices, indicesCount);
}

void Mesh::SetData(const float* combinedVerticesNormals, unsigned int combinedVerticesNormalsCount, const unsigned int* indices, unsigned int indicesCount) 
{
    m_indexCount = indicesCount;

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glBufferData(GL_ARRAY_BUFFER, 6 * combinedVerticesNormalsCount * sizeof(float), combinedVerticesNormals, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);;
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    

}

void Mesh::draw() const {
    glBindVertexArray(m_VAO);

    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}

}