#include "renderer/Buffer.h"

#include <glad/glad.h>

namespace sal
{
    static GLenum GetGLTarget(EBufferTarget target)
    {
        switch (target)
        {
            case EBufferTarget::VERTEX: return GL_ARRAY_BUFFER;
            case EBufferTarget::INDEX: return GL_ELEMENT_ARRAY_BUFFER;
        }

        ASSERT(false);
        return 0;
    }

    static GLenum GetGLUsage(EBufferUsage usage)
    {
        switch (usage)
        {
            case EBufferUsage::STATIC: return GL_STATIC_DRAW;
            case EBufferUsage::DYNAMIC: return GL_DYNAMIC_DRAW;
            case EBufferUsage::STREAM: return GL_STREAM_DRAW;
        }

        ASSERT(false);
        return 0;
    }

    size_t GetShaderDataTypeSize(EShaderDataType type)
    {
        switch (type)
        {
            case EShaderDataType::FLOAT: return sizeof(float);
            case EShaderDataType::FLOAT2: return sizeof(float) * 2;
            case EShaderDataType::FLOAT3: return sizeof(float) * 3;
            case EShaderDataType::FLOAT4: return sizeof(float) * 4;
        }

        ASSERT(false);
        return 0;
    }

    size_t GetShaderDataTypeCompCount(EShaderDataType type)
    {
        switch (type)
        {
            case EShaderDataType::FLOAT: return 1;
            case EShaderDataType::FLOAT2: return 2;
            case EShaderDataType::FLOAT3: return 3;
            case EShaderDataType::FLOAT4: return 4;
        }

        ASSERT(false);
        return 0;
    }

    void BufferLayout::Push(const std::string& name, EShaderDataType type)
    {
        m_size += GetShaderDataTypeSize(type);
        m_elements.emplace_back(name, type);
    }

    Buffer::Buffer(EBufferTarget target, size_t size, EBufferUsage usage)
    {
        GLenum glTarget = GetGLTarget(target);

        glGenBuffers(1, &m_handle);
        glBindBuffer(glTarget, m_handle);
        glBufferData(glTarget, size, NULL, GetGLUsage(usage));

        glBindBuffer(glTarget, 0);

        m_target = target;
    }

    Buffer::Buffer(EBufferTarget target, size_t size, void* data, EBufferUsage usage)
    {
        GLenum glTarget = GetGLTarget(target);

        glGenBuffers(1, &m_handle);
        glBindBuffer(glTarget, m_handle);
        glBufferData(glTarget, size, data, GetGLUsage(usage));

        glBindBuffer(glTarget, 0);

        m_target = target;
    }

    Buffer::~Buffer()
    {
        glDeleteBuffers(1, &m_handle);
    }

    void Buffer::Use()
    {
        glBindBuffer(GetGLTarget(m_target), m_handle);
    }

    void Buffer::SetData(size_t size, void* data)
    {
        glBufferSubData(GetGLTarget(m_target), 0, size, data);
    }
}