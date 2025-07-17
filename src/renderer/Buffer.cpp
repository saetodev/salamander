#include "renderer/IndexBuffer.h"
#include "renderer/VertexBuffer.h"

#include <glad/glad.h>

namespace sal {

    VertexBuffer::VertexBuffer(size_t size, const BufferLayout& layout) {
        glGenBuffers(1, &m_handle);
        glBindBuffer(GL_ARRAY_BUFFER, m_handle);
        glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        m_layout = layout;
    }

    VertexBuffer::VertexBuffer(size_t size, void* data, const BufferLayout& layout) {
        glGenBuffers(1, &m_handle);
        glBindBuffer(GL_ARRAY_BUFFER, m_handle);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        m_layout = layout;
    }

    VertexBuffer::~VertexBuffer() {
        glDeleteBuffers(1, &m_handle);
    }

    void VertexBuffer::Use() {
        glBindBuffer(GL_ARRAY_BUFFER, m_handle);
    }

    void VertexBuffer::SetData(size_t size, void* data) {
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    void VertexBuffer::BindAttributes() {
        int location  = 0;
        size_t offset = 0;

        for (const Attribute& attribute : m_layout) {
            glEnableVertexAttribArray(location);
            glVertexAttribPointer(location, GetDataTypeComponentCount(attribute.type), GL_FLOAT, false, m_layout.Size(), (const void*)offset);

            location += 1;
            offset   += GetDataTypeSize(attribute.type);
        }
    }

    IndexBuffer::IndexBuffer(size_t size) {
        glGenBuffers(1, &m_handle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    IndexBuffer::IndexBuffer(size_t size, void* data) {
        glGenBuffers(1, &m_handle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    IndexBuffer::~IndexBuffer() {
        glDeleteBuffers(1, &m_handle);
    }

    void IndexBuffer::Use() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
    }

    void IndexBuffer::SetData(size_t size, void* data) {
        glBufferSubData(m_handle, 0, size, data);
    }

}