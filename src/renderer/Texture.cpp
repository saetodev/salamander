#include "renderer/Texture.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace sal {

    Texture::Texture(int width, int height, void* data) {
        glGenTextures(1, &m_handle);
        glBindTexture(GL_TEXTURE_2D, m_handle);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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

    Ref<Texture> Texture::Load(std::string_view filename) {
        //TODO: this should be in some settings config
        stbi_set_flip_vertically_on_load(true);

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