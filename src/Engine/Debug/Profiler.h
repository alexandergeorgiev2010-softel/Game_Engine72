
#pragma once

#include <chrono>

namespace Engine {

    class Profiler
    {
    public:
        void BeginFrame();
        void EndFrame();

        void BeginUpdate();
        void EndUpdate();

        void BeginRender();
        void EndRender();

        float GetFrameTime() const;
        float GetUpdateTime() const;
        float GetRenderTime() const;

        float GetFPS() const;

        void PrintStats();

    private:
        using Clock = std::chrono::steady_clock;

        Clock::time_point m_frameStart;
        Clock::time_point m_updateStart;
        Clock::time_point m_renderStart;

        float m_frameTime = 0.0f;
        float m_updateTime = 0.0f;
        float m_renderTime = 0.0f;

        float m_fps = 0.0f;
        float m_printTimer = 0.0f;
    };

}
