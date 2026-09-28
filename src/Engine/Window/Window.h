#pragma once

#include "Engine/Math/Vec3.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <array>

namespace Engine {

class Window {
    public:
        Window(int width, int height, const char* title);
        ~Window();

        bool ShouldClose();
        void PollEvents();
        void SwapBuffers();

        Vec3 GetMouseDelta();
        void CaptureMouse();
        void ReleaseMouse();
        void ToggleMouseCapture();

        bool IsKeyPressed(int key);
        bool WasKeyPressed(int key);
        bool IsMouseCaptured();

        void GetWindowSize(int& width, int& height) const;

    private:
        void* m_window = nullptr;

        bool m_MouseCaptured = false;

        std::array<bool, GLFW_KEY_LAST + 1>m_PreviousKeyState{};

        double m_LastX = 0.0;
        double m_LastY = 0.0;
        bool m_FirstMouse = true;
};


}



