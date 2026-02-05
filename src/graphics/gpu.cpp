#include "graphics/gpu.h"

#include "glad/glad.h"

namespace sal::gpu {
    static GLenum glVertexFormat(VertexFormat format) {
        switch (format) {
            case VertexFormat::FLOAT:  return GL_FLOAT;
            case VertexFormat::FLOAT2: return GL_FLOAT;
            case VertexFormat::FLOAT3: return GL_FLOAT;
            case VertexFormat::FLOAT4: return GL_FLOAT;
        }

        ASSERT(false);
        return 0;
    }

    static GLint glVertexFormatSize(VertexFormat format) {
        switch (format) {
            case VertexFormat::FLOAT:  return sizeof(float);
            case VertexFormat::FLOAT2: return sizeof(float) * 2;
            case VertexFormat::FLOAT3: return sizeof(float) * 3;
            case VertexFormat::FLOAT4: return sizeof(float) * 4;
        }

        ASSERT(false);
        return 0;
    }

    static GLint glVertexFormatCount(VertexFormat format) {
        switch (format) {
            case VertexFormat::FLOAT:  return 1;
            case VertexFormat::FLOAT2: return 2;
            case VertexFormat::FLOAT3: return 3;
            case VertexFormat::FLOAT4: return 4;
        }

        ASSERT(false);
        return 0;
    }

    static GLenum glPrimitiveType(PrimitiveType type) {
        switch (type) {
            case PrimitiveType::LINE_LIST: return GL_LINES;
            case PrimitiveType::TRIANGLE_LIST: return GL_TRIANGLES;
        }

        ASSERT(false);
        return 0;
    }

    static GLenum glBufferType(BufferType type) {
        switch (type) {
            case BufferType::VERTEX: return GL_ARRAY_BUFFER;
            case BufferType::INDEX: return GL_ELEMENT_ARRAY_BUFFER;
        }

        ASSERT(false);
        return 0;
    }

    static GLenum glDrawBufferUsage(BufferUsage usage) {
        switch (usage) {
            case BufferUsage::STATIC: return GL_STATIC_DRAW;
            case BufferUsage::DYNAMIC: return GL_DYNAMIC_DRAW;
            case BufferUsage::STREAM: return GL_STREAM_DRAW;
        }

        ASSERT(false);
        return 0;
    }

    static GLenum glTextureFilter(TextureFilter filter) {
        switch (filter) {
            case TextureFilter::NEAREST: return GL_NEAREST;
            case TextureFilter::LINEAR: return GL_LINEAR;
        };

        ASSERT(false);
        return 0;
    }

    static GLenum glTextureWrap(TextureWrap wrap) {
        switch (wrap) {
            case TextureWrap::CLAMP: return GL_CLAMP_TO_EDGE;
            case TextureWrap::REPEAT: return GL_REPEAT;
            case TextureWrap::MIRROR_REPEAT: return GL_MIRRORED_REPEAT;
        };

        ASSERT(false);
        return 0;
    }

    static GLenum glPixelFormat(PixelFormat format) {
        switch (format) {
            case PixelFormat::RGB: return GL_RGB;
            case PixelFormat::RGBA: return GL_RGBA;
            case PixelFormat::LUMINANCE_ALPHA: return GL_LUMINANCE_ALPHA;
            case PixelFormat::LUMINANCE: return GL_LUMINANCE;
            case PixelFormat::ALPHA: return GL_ALPHA;
        }

        ASSERT(false);
        return 0;
    }

    Shader createShader(ShaderDesc desc) {
        GLint result = GL_FALSE;

        const char* vertexSource   = desc.vertexSource.c_str();
        const char* fragmentSource = desc.fragmentSource.c_str(); 

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSource, NULL);
        glCompileShader(vertexShader);

        //TODO: handle this properly
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
        ASSERT(result);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        glCompileShader(fragmentShader);

        //TODO: handle this properly
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
        ASSERT(result);

        GLuint program = glCreateProgram();

        for (int i = 0; i < desc.layout.names.size(); i++) {
            glBindAttribLocation(program, i, desc.layout.names[i].c_str());
        }

        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        //TODO: handle this properly
        glGetProgramiv(program, GL_LINK_STATUS, &result);
        ASSERT(result);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return program;
    }

    void destroyShader(Shader shader) {
        glDeleteProgram((GLuint)shader);
    }

    Texture createTexture(TextureDesc desc) {
        GLuint texture = 0;
        GLenum filter = glTextureFilter(desc.filter);
        GLenum wrap   = glTextureWrap(desc.wrap);
        GLenum format = glPixelFormat(desc.format);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

        glTexImage2D(GL_TEXTURE_2D, 0, format, desc.width, desc.height, 0, format, GL_UNSIGNED_BYTE, desc.pixels);
        glBindTexture(GL_TEXTURE_2D, 0);

        return (Texture)texture;
    }

    void destroyTexture(Texture texture) {
        glDeleteTextures(1, (GLuint*)&texture);
    }

    Buffer createBuffer(BufferDesc desc) {
        GLuint buffer = 0;
        GLenum target = glBufferType(desc.type);
        GLenum usage  = glDrawBufferUsage(desc.usage);

        glGenBuffers(1, &buffer);
        glBindBuffer(target, buffer);
        glBufferData(target, desc.size, desc.data, usage);
        glBindBuffer(target, 0);

        return (Buffer)buffer;
    }

    void destroyBuffer(Buffer buffer) {
        glDeleteBuffers(1, (GLuint*)&buffer);
    }

    void bind(Shader shader) {
        glUseProgram((GLuint)shader);
    }

    void bind(uint32_t unit, Texture texture) {
        ASSERT(unit < 32);
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    void bind(BufferType type, Buffer buffer) {
        glBindBuffer(glBufferType(type), (GLuint)buffer);
    }

    void bind(const VertexLayout& layout) {
        size_t offset = 0;
        
        for (uint32_t i = 0; i < layout.formats.size(); i++) {
            VertexFormat format = layout.formats[i];
            GLint count = glVertexFormatCount(format);
            GLenum type = glVertexFormat(format);

            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, count, type, GL_FALSE, layout.size, (const void*)offset);
        
            offset += glVertexFormatSize(format);
        }
    }

    void clear(float r, float g, float b, float a) {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void drawPrimitives(PrimitiveType primitive, uint32_t count) {
        glDrawArrays(glPrimitiveType(primitive), 0, count);
    }

    void drawPrimitivesIndexed(PrimitiveType primitive, uint32_t count) {
        glDrawElements(glPrimitiveType(primitive), count, GL_UNSIGNED_SHORT, NULL);
    }
}