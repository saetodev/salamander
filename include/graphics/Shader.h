#ifndef SAL_GRAPHICS_SHADER_H
#define SAL_GRAPHICS_SHADER_H

#include "graphics/gpu.h"

namespace sal {
    class Shader {
    public:
        Shader(gpu::ShaderDesc desc) {
            m_handle = gpu::createShader(desc);
        }

        //NOTE: not copyable
        Shader(const Shader& other) = delete;

        Shader(Shader&& other) {
            m_handle = other.m_handle;
            other.m_handle = {};
        }

        ~Shader() {
            gpu::destroyShader(m_handle);
        }

        //NOTE: not copyable
        Shader& operator=(const Shader& other) = delete;

        Shader& operator=(Shader&& other) {
            if (this != &other) {
                m_handle = other.m_handle;
                other.m_handle = {};
            }

            return *this;
        }

        gpu::ShaderHandle handle() const { return m_handle; }
    private:
        gpu::ShaderHandle m_handle = {};
    };
}

#endif