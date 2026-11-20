
#include "Profiler.h"

#include <iostream>
#include <iomanip>

namespace Engine {

    void Profiler::BeginFrame()
    {
        m_frameStart = Clock::now();
    }

    void Profiler::EndFrame()
    {
        auto frameEnd = Clock::now();

        m_frameTime =
            std::chrono::duration<float, std::milli>(
                frameEnd - m_frameStart
            ).count();

        if (m_frameTime > 0.0f)
            m_fps = 1000.0f / m_frameTime;

        m_printTimer += m_frameTime / 1000.0f;

        if (m_printTimer >= 1.0f)
        {
            PrintStats();
            m_printTimer = 0.0f;
        }
    }

    void Profiler::BeginUpdate()
    {
        m_updateStart = Clock::now();
    }

    void Profiler::EndUpdate()
    {
        auto end = Clock::now();

        m_updateTime =
            std::chrono::duration<float, std::milli>(
                end - m_updateStart
            ).count();
    }

    void Profiler::BeginRender()
    {
        m_renderStart = Clock::now();
    }

    void Profiler::EndRender()
    {
        auto end = Clock::now();

        m_renderTime =
            std::chrono::duration<float, std::milli>(
                end - m_renderStart
            ).count();
    }

    float Profiler::GetFrameTime() const
    {
        return m_frameTime;
    }

    float Profiler::GetUpdateTime() const
    {
        return m_updateTime;
    }

    float Profiler::GetRenderTime() const
    {
        return m_renderTime;
    }

    float Profiler::GetFPS() const
    {
        return m_fps;
    }

    void Profiler::PrintStats()
    {
        std::cout << std::fixed << std::setprecision(2);

        std::cout
            << "[PROFILER] "
            << "FPS: " << m_fps
            << " | Frame: " << m_frameTime << " ms"
            << " | Update: " << m_updateTime << " ms"
            << " | Render: " << m_renderTime << " ms"
            << std::endl;
    }

}
