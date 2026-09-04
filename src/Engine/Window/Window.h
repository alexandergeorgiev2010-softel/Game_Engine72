#pragma once

namespace Engine {

class Window {
    public:
        Window(int width, int height, const char* title);
        ~Window();

        bool ShouldClose();
        void PollEvents();

    private:
        void* m_window;
};


}



