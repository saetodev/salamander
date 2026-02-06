#include "audio/AudioDevice.h"
#include "core/App.h"
#include "core/Window.h"
#include "core/Input.h"
#include "graphics/Renderer2D.h"

namespace sal {

    App::App(const Settings& settings) {
        ASSERT(!s_instance); s_instance = this;

        m_settings = settings;

        m_window   = MakeScope<Window>();
        m_renderer = MakeScope<Renderer2D>();
        m_input    = MakeScope<Input>();
        m_audio    = MakeScope<AudioDevice>();
    }

    void App::Run() {
        m_window->Init(m_settings.windowWidth, m_settings.windowHeight, m_settings.windowTitle);
        m_renderer->Init();
        m_audio->Init();

        Init();

        while (m_window->Running()) {
            float delta = m_window->FrameTime();

            Update(delta);

            m_window->SwapBuffers();
        }

        Shutdown();

        m_audio->Shutdown();
        m_renderer->Shutdown();
        m_window->Shutdown();
    }

}