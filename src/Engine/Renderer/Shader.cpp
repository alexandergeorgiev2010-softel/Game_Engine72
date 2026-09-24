#include "Engine/Renderer/Shader.h"
#include "glad/gl.h"

#include <fstream>
#include <sstream>
#include <iostream>

namespace Engine {

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::ifstream vertexFile(vertexPath);
    
    std::stringstream vertexStream;
    vertexStream << vertexFile.rdbuf();

    std::string vertexSource = vertexStream.str();

    
    std::ifstream fragmentFile(fragmentPath);

    std::stringstream fragmentStream;
    fragmentStream << fragmentFile.rdbuf();

    std::string fragmentSource = fragmentStream.str();

    const char* vertexSourceCStr = vertexSource.c_str();
    const char* fragmentSourceCStr = fragmentSource.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexSourceCStr, nullptr);
    glShaderSource(fragmentShader, 1, &fragmentSourceCStr, nullptr);

    
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];

        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);

        std::cerr << "Failed compiling vertex shader\n" << infoLog << '\n';
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];

        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        
        std::cerr << "Failed compiling fragment shader\n" << infoLog << '\n';
    }

    m_program = glCreateProgram();

    glAttachShader(m_program, vertexShader);
    glAttachShader(m_program, fragmentShader);
    glLinkProgram(m_program);
    
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);

    if (!success) {
        char infoLog[512];

        glGetProgramInfoLog(m_program, 512, nullptr, infoLog);

        std::cerr << "Failed linking Shader Program\n" << infoLog << '\n';
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


}

void Shader::bind() const {
    glUseProgram(m_program);
}

}


