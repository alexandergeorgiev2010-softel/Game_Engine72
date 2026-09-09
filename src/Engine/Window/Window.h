#pragma once


namespace Engine {

class Window {
    public:
        Window(int width, int height, const char* title);
        ~Window();

        bool ShouldClose();
        void PollEvents();
        void SwapBuffers();

    private:
        void* m_window;
};


}



