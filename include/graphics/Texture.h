#ifndef SAL_GRAPHICS_TEXTURE_H
#define SAL_GRAPHICS_TEXTURE_H

#include "graphics/gpu.h"

namespace sal {
    class Texture {
    public:
        Texture(gpu::TextureDesc desc) {
            m_handle = gpu::createTexture(desc);
            m_width  = desc.width;
            m_height = desc.height;
        }

        //NOTE: not copyable
        Texture(const Texture& other) = delete;

        Texture(Texture&& other) {
            m_handle = other.m_handle;
            m_width  = other.m_width;
            m_height = other.m_height;

            other.m_handle = {};
        }

        ~Texture() {
            gpu::destroyTexture(m_handle);
        }

        //NOTE: not copyable
        Texture& operator=(const Texture& other) = delete;

        Texture& operator=(Texture&& other) {
            if (this != &other) {
                m_handle = other.m_handle;
                m_width  = other.m_width;
                m_height = other.m_height;

                other.m_handle = {};
            }

            return *this;
        }

        gpu::TextureHandle handle() const { return m_handle; }
        uint32_t width() const { return m_width; }
        uint32_t height() const { return m_height; }
    private:
        gpu::TextureHandle m_handle = {};

        uint32_t m_width  = 0;
        uint32_t m_height = 0;
    };
};

#endif