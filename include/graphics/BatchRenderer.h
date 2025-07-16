#pragma once

#include "graphics/BufferLayout.h"
#include "graphics/IndexBuffer.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "graphics/VertexBuffer.h"

#include <glm/glm.hpp>

namespace sal {

    struct QuadVertex;
    struct CircleVertex;
    struct LineVertex;

    enum Shape {
        None,
        Quad,
        Circle,
        Line,
    };

    class BatchRenderer {
    public:
        BatchRenderer();
        ~BatchRenderer();

        void Begin(const glm::mat4& projection);
        void End();

        void DrawRect(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        void DrawRect(const glm::mat4& transform, const glm::vec4& color);

        void DrawTexture(Ref<Texture> texture, const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        void DrawTexture(Ref<Texture> texture, const glm::mat4& transform, const glm::vec4& color);

        void DrawCircle(const glm::vec2& position, float radius, const glm::vec4& color);
        void DrawCircle(const glm::mat4& transform, const glm::vec4& color);

        void DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color);
    private:
        void StartBatch();
        void Flush();
        bool RequiresFlushForShape(Shape shape);
        bool RequiresFlushForTexture(Ref<Texture> texture);
    private:
        static constexpr int VERTICES_PER_QUAD = 4;
        static constexpr int INDICES_PER_QUAD  = 6;

        static constexpr int VERTICES_PER_LINE = 2;

        static constexpr int MAX_QUAD_COUNT   = 1024;
        static constexpr int MAX_VERTEX_COUNT = MAX_QUAD_COUNT * VERTICES_PER_QUAD;
        static constexpr int MAX_INDEX_COUNT  = MAX_QUAD_COUNT * INDICES_PER_QUAD;

        static constexpr glm::vec4 QUAD_VERTEX_POSITIONS[VERTICES_PER_QUAD] = {
            { -0.5f, -0.5f, 0.0f, 1.0f },
            {  0.5f, -0.5f, 0.0f, 1.0f },
            {  0.5f,  0.5f, 0.0f, 1.0f },
            { -0.5f,  0.5f, 0.0f, 1.0f },
        };

        static constexpr glm::vec2 QUAD_TEXTURE_COORDS[VERTICES_PER_QUAD] = {
            { 0.0f, 0.0f },
            { 1.0f, 0.0f },
            { 1.0f, 1.0f },
            { 0.0f, 1.0f },
        };

        // batch data

        glm::mat4 m_projection;

        Shape        m_batchShape   = Shape::None;
        Ref<Texture> m_batchTexture = {};

        // quad batch data

        Ref<VertexBuffer> m_quadVBO      = {};
        Ref<IndexBuffer>  m_quadIBO      = {};
        Ref<Shader>       m_quadShader   = {};
        Ref<Texture>      m_whiteTexture = {};

        uint32_t    m_quadCount            = 0;
        QuadVertex* m_quadVertexBufferBase = nullptr;
        QuadVertex* m_quadVertexBufferPtr  = nullptr;

        // circle batch data

        Ref<VertexBuffer> m_circleVBO    = {};
        Ref<IndexBuffer>  m_circleIBO    = {};
        Ref<Shader>       m_circleShader = {};

        uint32_t      m_circleCount            = 0;
        CircleVertex* m_circleVertexBufferBase = nullptr;
        CircleVertex* m_circleVertexBufferPtr  = nullptr;

        // line batch data

        Ref<VertexBuffer> m_lineVBO    = {};
        Ref<Shader>       m_lineShader = {};

        uint32_t m_lineCount               = 0;
        LineVertex* m_lineVertexBufferBase = nullptr;
        LineVertex* m_lineVertexBufferPtr  = nullptr;
    };

}