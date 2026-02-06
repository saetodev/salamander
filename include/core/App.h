#pragma once

#include "core/Window.h"
#include "core/Input.h"
#include "audio/AudioDevice.h"
#include "graphics/Renderer2D.h"

namespace sal {

    class Window;
    class Input;
    class AudioDevice;
    class Renderer2D;

    struct Settings {
        int         windowWidth  = 640;
        int         windowHeight = 480;
        const char* windowTitle  = "WINDOW";
    };

    class App {
    public:
        App(const Settings& settings = {});
        virtual ~App() = default;

        //TODO: texture loading hack
        gpu::Texture LoadTexture(const char* filename);

        void Run();

        virtual void Init()              = 0;
        virtual void Shutdown()          = 0;
        virtual void Update(float delta) = 0;

        static Window& GetWindow() { return *s_instance->m_window; }
        static Renderer2D& GetRenderer() { return *s_instance->m_renderer; }
        static Input& GetInput() { return *s_instance->m_input; }
        static AudioDevice& GetAudio() { return *s_instance->m_audio; }
    private:
        Settings m_settings = {};

        Scope<Window>        m_window   = {};
        Scope<Renderer2D> m_renderer = {};
        Scope<Input>         m_input    = {};
        Scope<AudioDevice>   m_audio    = {};

        static inline App* s_instance = nullptr;
    };

}
