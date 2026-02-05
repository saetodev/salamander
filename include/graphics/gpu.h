#ifndef SAL_GRAPHICS_GPU_H
#define SAL_GRAPHICS_GPU_H

#include "core/Base.h"

namespace sal::gpu {
    //TODO: replace these with structs for stronger typing
    using Shader  = uint32_t;
    using Texture = uint32_t;
    using Buffer  = uint32_t;

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

    struct VertexLayout {
        std::vector<std::string>  names;
        std::vector<VertexFormat> formats;
        size_t                    size;
    };

    struct ShaderDesc {
        std::string  vertexSource;
        std::string  fragmentSource;
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

    Shader createShader(ShaderDesc desc);
    void destroyShader(Shader shader);

    Texture createTexture(TextureDesc desc);
    void destroyTexture(Texture texture);

    Buffer createBuffer(BufferDesc desc);
    void destroyBuffer(Buffer buffer);

    void bind(Shader shader);
    void bind(uint32_t unit, Texture texture);
    void bind(BufferType type, Buffer buffer);
    void bind(const VertexLayout& layout);

    void clear(float r, float g, float b, float a);

    void drawPrimitives(PrimitiveType primitive, uint32_t count);
    void drawPrimitivesIndexed(PrimitiveType primitive, uint32_t count);
}

#endif