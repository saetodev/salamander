#pragma once

#include "renderer/Buffer.h"
#include "renderer/Camera.h"

namespace sal {

    class Buffer;
    class Shader;
    class Texture;

    namespace Color
    {
        inline glm::vec4 BLACK = { 0.0f, 0.0f, 0.0f, 1.0f };
        inline glm::vec4 WHITE = { 1.0f, 1.0f, 1.0f, 1.0f };
        inline glm::vec4 RED   = { 1.0f, 0.0f, 0.0f, 1.0f };
        inline glm::vec4 GREEN = { 0.0f, 1.0f, 0.0f, 1.0f };
        inline glm::vec4 BLUE  = { 0.0f, 0.0f, 1.0f, 1.0f };
    }

    class BatchRenderer {
    public:
        void Init();

        void Begin(const Camera& camera);
        void End();

        void DrawRect(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        void DrawRect(const glm::mat4& transform, const glm::vec4& color);        
        void DrawRectLines(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        void DrawRectLines(const glm::mat4& transform, const glm::vec4& color);

        void DrawTexture(const Ref<Texture>& texture, const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        void DrawTexture(const Ref<Texture>& texture, const glm::mat4& transform, const glm::vec4& color);
        void DrawTexture(const Ref<Texture>& texture, const glm::vec4& source, const glm::mat4& transform, const glm::vec4& color, const glm::vec2& tilingFactor = glm::vec2(1.0f, 1.0f));

        void DrawCircle(const glm::vec2& position, float radius, const glm::vec4& color);
        void DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color);

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
        bool RequiresFlushForTexture(const Ref<Texture>& texture);
    private:
        // batch state

        Camera m_camera = {};

        BatchMode    m_batchMode    = BatchMode::None;
        Ref<Texture> m_batchTexture = {};

        uint32_t  m_batchVertexCount = 0;
        uint32_t  m_batchIndexCount  = 0;
        
        // buffer data

        BufferLayout m_bufferLayout = {};
        Ref<Buffer>  m_batchVBO     = {};
        Ref<Buffer>  m_batchIBO     = {};
        Ref<Texture> m_whiteTexture = {};

        Vertex* m_batchVertexBufferBase = nullptr;
        Vertex* m_batchVertexBufferPtr  = nullptr;

        // shaders

        Ref<Shader> m_quadShader   = {};
        Ref<Shader> m_circleShader = {};
        Ref<Shader> m_lineShader   = {};

        // stats

        uint32_t m_numDrawCalls = 0;
    };

}