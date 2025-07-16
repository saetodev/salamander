#pragma once

#include "core/Base.h"

namespace sal {

    class Texture {
    public:
        Texture(int width, int height, void* data);
        ~Texture();

        void Use();

        int Width() const { return m_width; }
        int Height() const { return m_height; }
    private:
        uint32_t m_handle = 0;
        int      m_width  = 0;
        int      m_height = 0;
    };

}