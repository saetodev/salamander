#pragma once

#include "core/Base.h"

namespace sal {

    struct Window;
    struct BatchRenderer;

    struct Settings {
        int         windowWidth  = 640;
        int         windowHeight = 480;
        const char* windowTitle  = "WINDOW";
    };

    class App {
    public:
        App(const Settings& settings = {});

        void Run();

        virtual void Init()              = 0;
        virtual void Shutdown()          = 0;
        virtual void Update(float delta) = 0;
    protected:
        Scope<Window>        m_window   = {};
        Scope<BatchRenderer> m_renderer = {};
    };

}