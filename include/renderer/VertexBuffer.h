#pragma once

#include "renderer/BufferLayout.h"

namespace sal {

    class VertexBuffer {
    public:
        VertexBuffer(size_t size, const BufferLayout& layout);
        VertexBuffer(size_t size, void* data, const BufferLayout& layout);
        ~VertexBuffer();

        void Use();
        void SetData(size_t size, void* data);
        void BindAttributes();

        const BufferLayout& Layout() const { return m_layout; }
    private:
        uint32_t     m_handle = 0;
        BufferLayout m_layout = {};
    };

}