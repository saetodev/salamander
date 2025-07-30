#pragma once

#include "renderer/Buffer.h"

namespace sal {

    class Shader {
    public:
        Shader(std::string_view vertexSource, std::string_view fragSource, const BufferLayout& layout);
        ~Shader();

        void Use();

        void SetInt(std::string_view name, int value);
        void SetFloat(std::string_view name, float value);
        void SetVec2(std::string_view name, const glm::vec2& value);
        void SetVec3(std::string_view name, const glm::vec3& value);
        void SetVec4(std::string_view name, const glm::vec4& value);
        void SetMat4(std::string_view name, const glm::mat4& value);
    private:
        uint32_t m_handle = 0;
    };
    
}
