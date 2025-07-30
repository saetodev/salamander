#include "renderer/RenderAPI.h"
#include "renderer/Texture.h"

#include <glad/glad.h>

namespace sal {

    static GLenum GetGLPrimitive(Primitive type) {
        switch (type) {
            case Primitive::Lines:     return GL_LINES;
            case Primitive::Triangles: return GL_TRIANGLES;
        }

        assert(false);
        return 0;
    }

    static GLenum GetGLBlendFactor(BlendFactor factor) {
        switch (factor) {
            case Zero:             return GL_ZERO;
            case One:              return GL_ONE;
            case SrcAlpha:         return GL_SRC_ALPHA;
            case OneMinusScrAlpha: return GL_ONE_MINUS_SRC_ALPHA;
            case DstAlpha:         return GL_DST_ALPHA;
            case OneMinusDstAlpha: return GL_ONE_MINUS_DST_ALPHA;
            case SrcColor:         return GL_SRC_COLOR;
            case OneMinusSrcColor: return GL_ONE_MINUS_SRC_COLOR;
            case DstColor:         return GL_DST_COLOR;
            case OneMinusDstColor: return GL_ONE_MINUS_DST_COLOR;
        }

        assert(false);
        return 0;
    }

    void RenderAPI::EnableBlending(BlendFactor sfactor, BlendFactor dfactor) {
        glEnable(GL_BLEND);
        glBlendFunc(GetGLBlendFactor(sfactor), GetGLBlendFactor(dfactor));
    }

    void RenderAPI::Clear(const glm::vec4& color) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void RenderAPI::DrawBuffer(Primitive type, size_t count) {
        glDrawArrays(GetGLPrimitive(type), 0, count);
    }

    void RenderAPI::DrawBufferIndexed(Primitive type, size_t count) {
        glDrawElements(GetGLPrimitive(type), count, GL_UNSIGNED_SHORT, nullptr);
    }

    void RenderAPI::BindTextureUnit(Ref<Texture> texture, int unit) {
        assert(unit >= 0 && unit < 32);

        glActiveTexture(GL_TEXTURE0 + unit);
        texture->Use();
    }

    void RenderAPI::BindVertexAttribs(const BufferLayout& layout)
    {
        size_t index  = 0;
        size_t offset = 0;

        for (const BufferElement& element : layout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, GetShaderDataTypeCompCount(element.type), GL_FLOAT, false, layout.Size(), (const void*)offset);

            index  += 1;
            offset += GetShaderDataTypeSize(element.type);
        }
    }

}