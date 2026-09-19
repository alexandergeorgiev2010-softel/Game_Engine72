#pragma once

#include <glad/gl.h>

class Shader {
    public:
        Shader(const char* vertexPath, const char* fragmentPath);

        void bind() const;
    
    private:
        unsigned int m_program = 0;
};

void Shader::bind() const {
    glUseProgram(m_program);
}