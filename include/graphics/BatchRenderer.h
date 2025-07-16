#pragma once

#include "graphics/BufferLayout.h"
#include "graphics/IndexBuffer.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "graphics/VertexBuffer.h"

namespace sal {

    class BatchRenderer {
    public:
        BatchRenderer();

        void Begin(const glm::mat4& projection);
        void End();

        void DrawRect(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        void DrawRect(const glm::mat4& transform, const glm::vec4& color);

        void DrawRectLines(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        void DrawRectLines(const glm::mat4& transform, const glm::vec4& color);

        void DrawTexture(Ref<Texture> texture, const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        void DrawTexture(Ref<Texture> texture, const glm::mat4& transform, const glm::vec4& color);

        void DrawCircle(const glm::vec2& position, float radius, const glm::vec4& color);
        void DrawCircle(const glm::mat4& transform, const glm::vec4& color);

        void DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color);

        uint32_t NumDrawCalls() const { return m_numDrawCalls; }
    private:
        struct Vertex {
            glm::vec4 position;
            glm::vec4 color;
            glm::vec2 textureCoord;  // used for textures
            glm::vec2 localPosition; // used for circles
        };

        enum BatchShape {
            None,
            Quad,
            Circle,
            Line,
        };

        void StartBatch();
        void Flush();

        bool RequiresFlushForSpace();
        bool RequiresFlushForShape(BatchShape shape);
        bool RequiresFlushForTexture(Ref<Texture> texture);
    private:
        glm::mat4 m_projection;

        BatchShape   m_batchShape   = BatchShape::None;
        Ref<Texture> m_batchTexture = {};

        Ref<VertexBuffer> m_batchVBO     = {};
        Ref<IndexBuffer>  m_batchIBO     = {};
        Ref<Texture>      m_whiteTexture = {};

        Ref<Shader> m_quadShader   = {};
        Ref<Shader> m_circleShader = {};
        Ref<Shader> m_lineShader   = {};

        uint32_t m_batchVertexCount             = 0;
        uint32_t m_batchIndexCount              = 0;
        uint32_t m_batchVertexBufferOffset      = 0;
        Scope<Vertex[]> m_batchVertexBufferBase = {};

        uint32_t m_numDrawCalls = 0;
    };

}