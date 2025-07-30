#pragma once

struct GLFWwindow;

namespace sal {

    class Window {
    public:
        void Init(int width, int height, const char* title);
        void Shutdown();

        void SwapBuffers();
        bool Running();

        int Width() const { return m_width; }
        int Height() const { return m_height; }

        float XScale() const { return m_xscale; }
        float YScale() const { return m_yscale; }

        float FrameTime() const { return m_deltaTime; }
    private:
        GLFWwindow* m_handle = nullptr;

        int m_width  = 0;
        int m_height = 0;

        float m_xscale = 0.0f;
        float m_yscale = 0.0f;

        //TODO: this might be a little out of place
        float m_lastTime  = 0.0f;
        float m_deltaTime = 0.0f;
    };

}