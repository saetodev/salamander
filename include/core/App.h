#pragma once

#include "core/Base.h"

namespace sal {

    struct Window;
    struct BatchRenderer;
    struct Input;

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

        static Window& GetWindow() { return *s_instance->m_window; }
        static BatchRenderer& GetRenderer() { return *s_instance->m_renderer; }
        static Input& GetInput() { return *s_instance->m_input; }
    private:
        Scope<Window>        m_window   = {};
        Scope<BatchRenderer> m_renderer = {};
        Scope<Input>         m_input    = {};

        static inline App* s_instance = nullptr;
    };

}