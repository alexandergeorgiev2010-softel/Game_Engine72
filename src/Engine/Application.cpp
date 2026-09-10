#include "Engine/Application.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <chrono>
#include <iostream>
#include <thread>
#include <utility>


namespace Engine {

Application::Application(ApplicationConfig config)
    : m_config(std::move(config)),
      m_window(1600, 900, "Game Engine 72"),
      m_RigidBody({0, 2, 0}, {0, 0, 0}, 1.0f, false),
      m_PhysicsWorld()
{
}

void Application::run()
{
    std::cout << "Starting " << m_config.name << '\n';
     


    auto lastFrameTime = std::chrono::steady_clock::now();

    while (m_running) {
        m_window.PollEvents();

        const auto currentFrameTime = std::chrono::steady_clock::now();
        const std::chrono::duration<float> frameDuration = currentFrameTime - lastFrameTime;
        const float deltaTime = frameDuration.count();
        lastFrameTime = currentFrameTime;

        update(deltaTime, m_RigidBody);
        render();

        ++m_frame;
        if (/*m_frame >= m_config.maxFrames ||*/ m_window.ShouldClose()) {
            m_running = false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    std::cout << "Application stopped.\n";
}

void Application::update(float deltaTime, RigidBody& m_RigidBody)
{
   m_PhysicsWorld.update(deltaTime, m_RigidBody);
}

void Application::render()
{
    std::cout << "Render frame " << m_frame << '\n';

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSource = R"(
        #version 410 core
        layout (location = 0) in vec3 aPos;

        void main() {
            gl_Position = vec4(aPos, 1.0);
        }
        
        )";

    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation failed:\n" << infoLog << '\n';
    }else {
        std::cout << "Vertex shader compiled successfully.\n";
    }


    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSource = R"(
        #version 410 core

        out vec4 FragColor;

        void main() {
            FragColor = vec4(0.0, 1.0, 0.0, 0.7);
        }

        )";

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed\n" << infoLog << '\n'; 
    }else {
        std::cout << "Fragment shader compiled successfully\n";
    }

    GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed\n" << infoLog << '\n';
    }else {
        std::cout << "Shader program linked successfully\n";
    }

    glUseProgram(shaderProgram);
    glDrawArrays(GL_TRIANGLES, 0, 3);




    m_window.SwapBuffers();
}

} 
