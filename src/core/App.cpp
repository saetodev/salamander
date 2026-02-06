#include "audio/AudioDevice.h"
#include "core/App.h"
#include "core/Window.h"
#include "core/Input.h"
#include "graphics/Renderer2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace sal {

    App::App(const Settings& settings) {
        ASSERT(!s_instance); s_instance = this;

        m_settings = settings;

        m_window   = MakeScope<Window>();
        m_renderer = MakeScope<Renderer2D>();
        m_input    = MakeScope<Input>();
        m_audio    = MakeScope<AudioDevice>();
    }

    Ref<Texture> App::LoadTexture(const char* filename) {
        int width  = 0;
        int height = 0;
        int comp   = 0;

        uint8_t* data = stbi_load(filename, &width, &height, &comp, 4);

        if (!data) {
            //TODO: error here
            return {};
        }

        gpu::TextureDesc texDesc = {
            .filter = gpu::TextureFilter::NEAREST,
            .wrap   = gpu::TextureWrap::CLAMP,
            .format = gpu::PixelFormat::RGBA,
            .width  = (uint32_t)width,
            .height = (uint32_t)height,
            .pixels = data,
        };

        Ref<Texture> texture = MakeRef<Texture>(texDesc);
        
        stbi_image_free(data);

        return texture;
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