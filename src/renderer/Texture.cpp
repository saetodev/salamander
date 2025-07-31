#include "renderer/Texture.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace sal {

    static GLenum GetGLFilterMode(ETextureFilter filter)
    {
        switch (filter)
        {
            case ETextureFilter::NEAREST: return GL_NEAREST;
            case ETextureFilter::LINEAR: return GL_LINEAR;
        };

        ASSERT(false);
        return 0;
    }

    static GLenum GetGLWrapMode(ETextureWrap wrap)
    {
        switch (wrap)
        {
            case ETextureWrap::CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
            case ETextureWrap::REPEAT: return GL_REPEAT;
            case ETextureWrap::MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
        }

        ASSERT(false);
        return 0;
    }

    Texture::Texture(int width, int height, void* data, ETextureFilter filter, ETextureWrap wrap) {
        glGenTextures(1, &m_handle);
        glBindTexture(GL_TEXTURE_2D, m_handle);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetGLFilterMode(filter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetGLFilterMode(filter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetGLWrapMode(wrap));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetGLWrapMode(wrap));

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);

        m_width  = width;
        m_height = height; 
    }

    Texture::~Texture() {
        glDeleteTextures(1, &m_handle);
    }

    void Texture::Use() {
        glBindTexture(GL_TEXTURE_2D, m_handle);
    }

    Ref<Texture> Texture::Load(std::string_view filename, bool verticalFlip) {
        stbi_set_flip_vertically_on_load(verticalFlip);

        int width  = 0;
        int height = 0;
        int comp   = 0;

        uint8_t* data = stbi_load(filename.data(), &width, &height, &comp, 4);

        if (!data) {
            //TODO: error here
            return {};
        }

        Ref<Texture> texture = MakeRef<Texture>(width, height, data);
        
        stbi_image_free(data);

        return texture;
    }

}