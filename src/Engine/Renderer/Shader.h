#pragma once

#include <glad/gl.h>

namespace Engine {

class Shader {
    public:
        Shader(const char* vertexPath, const char* fragmentPath);

        void bind() const;
    
    private:
        unsigned int m_program = 0;
};

}
