#include "core/App.h"
#include "core/Window.h"
#include "renderer/BatchRenderer.h"

namespace sal {

    App::App(const Settings& settings) {
        m_window   = MakeScope<Window>(settings.windowWidth, settings.windowHeight, settings.windowTitle);
        m_renderer = MakeScope<BatchRenderer>(); 
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