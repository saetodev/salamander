#ifndef SAL_GRAPHICS_GPU_H
#define SAL_GRAPHICS_GPU_H

#include "core/Base.h"

namespace sal::gpu {
    struct ShaderHandle { uint32_t id; };
    struct TextureHandle { uint32_t id; };
    struct BufferHandle { uint32_t id; };

    enum class VertexFormat {
        FLOAT,
        FLOAT2,
        FLOAT3,
        FLOAT4,
    };

    enum class PrimitiveType {
        LINE_LIST,
        TRIANGLE_LIST,
    };

    enum class BufferType {
        VERTEX,
        INDEX,
    };

    enum class BufferUsage {
        STATIC,
        DYNAMIC,
        STREAM,
    };

    enum class TextureFilter {
        NEAREST,
        LINEAR,
    };

    enum class TextureWrap {
        CLAMP,
        REPEAT,
        MIRROR_REPEAT,
    };

    enum class PixelFormat {
        RGB,
        RGBA,
        LUMINANCE_ALPHA,
        LUMINANCE,
        ALPHA,
    };

    struct VertexAttribute {
        VertexFormat format;
        const char*  name;
    };

    //NOTE: the underlying VertexAttribute array should be
    //      allocated on the heap
    struct VertexLayout {
        size_t           count;
        size_t           size;
        VertexAttribute* attributes;
    };

    struct ShaderDesc {
        const char*  vertexSource;
        const char*  fragmentSource;
        VertexLayout layout;
    };

    struct TextureDesc {
        TextureFilter filter;
        TextureWrap   wrap;
        PixelFormat   format;
        uint32_t      width;
        uint32_t      height;
        void*         pixels;
    };

    struct BufferDesc {
        BufferType  type;
        BufferUsage usage;
        size_t      size;
        void*       data;
    };

    ShaderHandle createShader(ShaderDesc desc);
    void destroyShader(ShaderHandle shader);

    void setShaderUniform(ShaderHandle shader, const char* name, float value);
    void setShaderUniform(ShaderHandle shader, const char* name, glm::vec2 value);
    void setShaderUniform(ShaderHandle shader, const char* name, glm::vec3 value);
    void setShaderUniform(ShaderHandle shader, const char* name, glm::vec4 value);
    void setShaderUniform(ShaderHandle shader, const char* name, glm::mat4 value);

    TextureHandle createTexture(TextureDesc desc);
    void destroyTexture(TextureHandle texture);

    BufferHandle createBuffer(BufferDesc desc);
    void destroyBuffer(BufferHandle buffer);
    void setBufferData(BufferType type, BufferHandle, size_t size, void* data);

    void bind(ShaderHandle shader);
    void bind(uint32_t unit, TextureHandle texture);
    void bind(BufferType type, BufferHandle buffer);
    void bind(const VertexLayout& layout);

    void clear(float r, float g, float b, float a);

    void drawPrimitives(PrimitiveType primitive, uint32_t count);
    void drawPrimitivesIndexed(PrimitiveType primitive, uint32_t count);
}

#endif