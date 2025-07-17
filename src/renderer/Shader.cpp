#include "renderer/Shader.h"

#include <iostream>

#include <glad/glad.h>

namespace sal {

    Shader::Shader(std::string_view vertexSource, std::string_view fragSource, const BufferLayout& layout) {
        int result = 0;

        const char* vertexSourceRaw = vertexSource.data();
        const char* fragSourceRaw   = fragSource.data();

        uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSourceRaw, NULL);
        glCompileShader(vertexShader);

        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);

        if (result == GL_FALSE) {
            int messageLen = 0;
            char* message  = new char[256];
            
            glGetShaderInfoLog(vertexShader, 255, &messageLen, message);

            std::cout << "Failed to compile vertex shader." << std::endl;
            std::cout << message << std::endl;

            delete message;

            glDeleteShader(vertexShader);

            return;
        }

        uint32_t fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragShader, 1, &fragSourceRaw, NULL);
        glCompileShader(fragShader);

        glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);

        if (result == GL_FALSE) {
            int messageLen = 0;
            char* message  = new char[256];
            
            glGetShaderInfoLog(fragShader, 255, &messageLen, message);

            std::cout << "Failed to compile fragment shader." << std::endl;
            std::cout << message << std::endl;

            delete message;

            glDeleteShader(vertexShader);
            glDeleteShader(fragShader);

            return;
        }

        m_handle = glCreateProgram();

        int location = 0;

        for (const Attribute& attribute : layout) {
            glBindAttribLocation(m_handle, location, attribute.name.c_str());
            location++;
        }

        glAttachShader(m_handle, vertexShader);
        glAttachShader(m_handle, fragShader);
        glLinkProgram(m_handle);
    }

    Shader::~Shader() {
        glDeleteProgram(m_handle);
    }

    void Shader::Use() {
        glUseProgram(m_handle);
    }

    void Shader::SetInt(std::string_view name, int value) {
        int loc = glGetUniformLocation(m_handle, name.data());
        glUniform1i(loc, value);
    }

    void Shader::SetFloat(std::string_view name, float value) {
        int loc = glGetUniformLocation(m_handle, name.data());
        glUniform1f(loc, value);
    }

    void Shader::SetVec2(std::string_view name, const glm::vec2& value) {
        int loc = glGetUniformLocation(m_handle, name.data());
        glUniform2fv(loc, 1, glm::value_ptr(value));
    }

    void Shader::SetVec3(std::string_view name, const glm::vec3& value) {
        int loc = glGetUniformLocation(m_handle, name.data());
        glUniform3fv(loc, 1, glm::value_ptr(value));
    }

    void Shader::SetVec4(std::string_view name, const glm::vec4& value) {
        int loc = glGetUniformLocation(m_handle, name.data());
        glUniform4fv(loc, 1, glm::value_ptr(value));
    }

    void Shader::SetMat4(std::string_view name, const glm::mat4& value) {
        int loc = glGetUniformLocation(m_handle, name.data());
        glUniformMatrix4fv(loc, 1, false, glm::value_ptr(value));
    }
    
}