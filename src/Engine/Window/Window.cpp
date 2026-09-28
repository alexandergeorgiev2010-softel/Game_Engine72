#include "Engine/Window/Window.h"
#include <glad/gl.h>
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

    Vec3 Window::GetMouseDelta() {
        double currentX;
        double currentY;

        glfwGetCursorPos(
            static_cast<GLFWwindow*>(m_window),
            &currentX,
            &currentY
        );

        if (m_FirstMouse) {
            m_LastX = currentX;
            m_LastY = currentY;
            m_FirstMouse = false;

            return Vec3(0.0f, 0.0f, 0.0f);
        }

        double deltaX = currentX - m_LastX;
        double deltaY = currentY - m_LastY;

        m_LastX = currentX;
        m_LastY = currentY;

        return Vec3(
            static_cast<float>(deltaX),
            static_cast<float>(deltaY),
            0.0f
        );
    }

    void Window::CaptureMouse() {
        glfwSetInputMode(
            static_cast<GLFWwindow*>(m_window),
            GLFW_CURSOR,
            GLFW_CURSOR_DISABLED
        );

        m_MouseCaptured = true;
        m_FirstMouse = true;
    }

    void Window::ReleaseMouse() {
        glfwSetInputMode(
            static_cast<GLFWwindow*>(m_window),
            GLFW_CURSOR,
            GLFW_CURSOR_NORMAL
        );

        m_MouseCaptured = false;
    }

    void Window::ToggleMouseCapture() {
        if (m_MouseCaptured) {
            ReleaseMouse();
        }else {
            CaptureMouse();
        }
    }

    bool Window::IsKeyPressed(int key) {
        return glfwGetKey(
            static_cast<GLFWwindow*>(m_window),
            key
        ) == GLFW_PRESS;
    }

    bool Window::WasKeyPressed(int key) {
        bool currentState = IsKeyPressed(key);

        bool justPressed = currentState && !m_PreviousKeyState[key];

        m_PreviousKeyState[key] = currentState;

        return justPressed;
    }

    void Window::GetWindowSize(int& width, int& height) const {
        glfwGetWindowSize(
            static_cast<GLFWwindow*>(m_window),
            &width,
            &height
        );
    }

    bool Window::IsMouseCaptured() {
        return m_MouseCaptured;
    }
}