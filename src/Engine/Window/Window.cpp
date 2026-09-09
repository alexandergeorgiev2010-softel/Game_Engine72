#include "Engine/Window/Window.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>


namespace Engine {
    Window::Window(int width, int height, const char* title) {
        if (!glfwInit()) {
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);

        if (!m_window) {
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_window));

        if (!gladLoadGL(GLADloadfunc(glfwGetProcAddress))) {
            std::cout << "Failed to initialize GLAD" << "\n";
            return;
        }

        std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
    }

    Window::~Window() {
        if (m_window != nullptr) {
            glfwDestroyWindow(static_cast<GLFWwindow*>(m_window));
        }

        glfwTerminate();
    }

    bool Window::ShouldClose() {
        return glfwWindowShouldClose(static_cast<GLFWwindow*>(m_window));
    }

    void Window::PollEvents() {
        glfwPollEvents();
    }

    void Window::SwapBuffers() {
        glfwSwapBuffers(static_cast<GLFWwindow*>(m_window));
    }

    
}