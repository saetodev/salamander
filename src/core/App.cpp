#include "core/App.h"
#include "core/Window.h"
#include "renderer/BatchRenderer.h"
#include "core/Input.h"

namespace sal {

    App::App(const Settings& settings) {
        ASSERT(!s_instance); s_instance = this;

        m_settings = settings;

        m_window   = MakeScope<Window>();
        m_renderer = MakeScope<BatchRenderer>();
        m_input    = MakeScope<Input>();
    }

    void App::Run() {
        m_window->Init(m_settings.windowWidth, m_settings.windowHeight, m_settings.windowTitle);
        m_renderer->Init();

        Init();

        while (m_window->Running()) {
            float delta = m_window->FrameTime();

            Update(delta);

            m_window->SwapBuffers();
        }

        Shutdown();

        m_window->Shutdown();
    }

}