#include "graphics/Texture.h"

#include "glad/glad.h"

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

}