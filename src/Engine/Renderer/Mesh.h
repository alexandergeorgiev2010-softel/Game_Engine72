#pragma once
#include <glad/gl.h>


namespace Engine {

class Mesh {
    public:
        Mesh();

        Mesh(
            const float* vertices,
            unsigned int vertexCount,
            const unsigned int* indices,
            unsigned int indicesCount
        );
            
        void draw() const;
    
    private:
        unsigned int m_VAO = 0;
        unsigned int m_VBO = 0;
        unsigned int m_EBO = 0;
        unsigned int m_indexCount = 0;
};


}