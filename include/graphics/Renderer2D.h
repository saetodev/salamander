#ifndef SAL_GRAPHICS_RENDERER2D_H
#define SAL_GRAPHICS_RENDERER2D_H

#include "graphics/Camera.h"
#include "graphics/gpu.h"
#include "graphics/Shader.h"
#include "graphics/Buffer.h"
#include "graphics/Texture.h"

namespace sal {
    namespace Color
    {
        inline glm::vec4 BLACK = { 0.0f, 0.0f, 0.0f, 1.0f };
        inline glm::vec4 WHITE = { 1.0f, 1.0f, 1.0f, 1.0f };
        inline glm::vec4 RED   = { 1.0f, 0.0f, 0.0f, 1.0f };
        inline glm::vec4 GREEN = { 0.0f, 1.0f, 0.0f, 1.0f };
        inline glm::vec4 BLUE  = { 0.0f, 0.0f, 1.0f, 1.0f };
    }

    class Renderer2D {
    public:
        void Init();
        void Shutdown();

        void Begin(const Camera& camera);
        void End();

        void DrawRect(glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color);    
        void DrawTexture(Ref<Texture> texture, glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color);

        void DrawCircle(glm::vec2 position, float radius, glm::vec4 color);
        void DrawLine(glm::vec2 start, glm::vec2 end, glm::vec4 color);

        uint32_t NumDrawCalls() const { return m_numDrawCalls; }
    private:
        struct Vertex {
            glm::vec4 position;
            glm::vec4 color;
            glm::vec2 textureCoord;  // used for textures
            glm::vec2 localPosition; // used for circles
        };

        enum BatchMode {
            None,
            Quad,
            Circle,
            Line,
        };

        void StartBatch();
        void Flush();

        bool RequiresFlushForSpace();
        bool RequiresFlushForMode(BatchMode mode);
        bool RequiresFlushForTexture(gpu::TextureHandle texture);
    private:
        Camera m_camera = {};

        BatchMode          m_batchMode    = BatchMode::None;
        gpu::TextureHandle m_batchTexture = {};

        gpu::VertexLayout   m_layout       = {};
        Scope<VertexBuffer> m_batchVBO     = {};
        Scope<IndexBuffer>  m_batchIBO     = {};
        Ref<Texture>        m_whiteTexture = {};

        Scope<Shader> m_quadShader   = {};
        Scope<Shader> m_circleShader = {};
        Scope<Shader> m_lineShader   = {};

        Vertex* m_vertexBufferBase = nullptr;
        Vertex* m_vertexBufferPtr  = nullptr;

        uint32_t m_vertexCount = 0;
        uint32_t m_indexCount  = 0;
        uint32_t m_numDrawCalls = 0;
    };
}

#endif