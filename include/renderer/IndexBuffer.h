#pragma once

#include "core/Base.h"

namespace sal {

    class IndexBuffer {
    public:
        IndexBuffer(size_t size);
        IndexBuffer(size_t size, void* data);
        ~IndexBuffer();
        
        void Use();
        void SetData(size_t size, void* data);
    private:
        uint32_t m_handle = 0;
    };

}