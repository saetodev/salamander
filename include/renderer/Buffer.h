#pragma once

#include "core/Base.h"

namespace sal
{
    enum class EBufferTarget
    {
        NONE,
        VERTEX,
        INDEX,
    };

    enum class EBufferUsage
    {
        STATIC,
        DYNAMIC,
        STREAM,
    };

    enum class EShaderDataType
    {
        FLOAT,
        FLOAT2,
        FLOAT3,
        FLOAT4,
    };

    struct BufferElement
    {
        std::string     name;
        EShaderDataType type;
    };

    class BufferLayout
    {
    public:
        using CIterator = std::vector<BufferElement>::const_iterator;

        void Push(const std::string& name, EShaderDataType type);

        size_t Size() const { return m_size; }

        CIterator begin() const { return m_elements.cbegin(); }
        CIterator end() const { return m_elements.cend(); }
    private:
        size_t m_size = 0;
        std::vector<BufferElement> m_elements = {};
    };

    class Buffer
    {
    public:
        Buffer(EBufferTarget target, size_t size, EBufferUsage usage);
        Buffer(EBufferTarget target, size_t size, void* data, EBufferUsage usage);
        ~Buffer();

        void Use();
        void SetData(size_t size, void* data);
    private:
        uint32_t      m_handle = 0;
        EBufferTarget m_target = EBufferTarget::NONE;
    };

    size_t GetShaderDataTypeSize(EShaderDataType type);
    size_t GetShaderDataTypeCompCount(EShaderDataType type);
}