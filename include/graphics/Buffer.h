#ifndef SAL_GRAPHICS_BUFFER_H
#define SAL_GRAPHICS_BUFFER_H

#include "graphics/gpu.h"

namespace sal {
    class VertexBuffer {
    public:
        VertexBuffer(gpu::BufferDesc desc) {
            ASSERT(desc.type == gpu::BufferType::VERTEX);
            m_handle = gpu::createBuffer(desc);
        }

        //NOTE: not copyable
        VertexBuffer(const VertexBuffer& other) = delete;

        VertexBuffer(VertexBuffer&& other) {
            m_handle = other.m_handle;
            other.m_handle = {};
        }

        ~VertexBuffer() {
            gpu::destroyBuffer(m_handle);
        }

        //NOTE: not copyable
        VertexBuffer& operator=(const VertexBuffer& other) = delete;

        VertexBuffer& operator=(VertexBuffer&& other) {
            if (this != &other) {
                m_handle = other.m_handle;
                other.m_handle = {};
            }

            return *this;
        }

        gpu::BufferHandle handle() const { return m_handle; }
    private:
        gpu::BufferHandle m_handle = {};
    };

    class IndexBuffer {
    public:
        IndexBuffer(gpu::BufferDesc desc) {
            ASSERT(desc.type == gpu::BufferType::INDEX);
            m_handle = gpu::createBuffer(desc);
        }

        //NOTE: not copyable
        IndexBuffer(const IndexBuffer& other) = delete;

        IndexBuffer(IndexBuffer&& other) {
            m_handle = other.m_handle;
            other.m_handle = {};
        }

        ~IndexBuffer() {
            gpu::destroyBuffer(m_handle);
        }

        //NOTE: not copyable
        IndexBuffer& operator=(const IndexBuffer& other) = delete;

        IndexBuffer& operator=(IndexBuffer&& other) {
            if (this != &other) {
                m_handle = other.m_handle;
                other.m_handle = {};
            }

            return *this;
        }

        gpu::BufferHandle handle() const { return m_handle; }
    private:
        gpu::BufferHandle m_handle = {};
    };
}

#endif