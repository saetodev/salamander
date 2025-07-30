#include "core/App.h"
#include "core/Window.h"
#include "renderer/BatchRenderer.h"
#include "core/Input.h"

namespace sal {

    App::App(const Settings& settings) {
        ASSERT(!s_instance);

        s_instance = this;

        m_window   = MakeScope<Window>(settings.windowWidth, settings.windowHeight, settings.windowTitle);
        m_renderer = MakeScope<BatchRenderer>();
        m_input    = MakeScope<Input>();
    }

    void App::Run() {
        Init();

        while (m_window->Running()) {
            float delta = m_window->FrameTime();

            Update(delta);

            m_window->SwapBuffers();
        }

        Shutdown();
    }

}