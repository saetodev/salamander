#pragma once

#include "core/Base.h"

namespace sal {

    enum ETextureFilter
    {
        NEAREST,
        LINEAR,
    };

    enum ETextureWrap
    {
        CLAMP_TO_EDGE,
        REPEAT,
        MIRRORED_REPEAT,
    };

    class Texture {
    public:
        Texture(int width, int height, void* data, ETextureFilter filter = ETextureFilter::NEAREST, ETextureWrap wrap = ETextureWrap::CLAMP_TO_EDGE);
        ~Texture();

        void Use();

        int Width() const { return m_width; }
        int Height() const { return m_height; }

        static Ref<Texture> Load(std::string_view filename, bool verticalFlip = false);
    private:
        uint32_t m_handle = 0;
        int      m_width  = 0;
        int      m_height = 0;
    };

}