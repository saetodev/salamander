#include "graphics/BatchRenderer.h"

#include <glad/glad.h>

namespace sal {

    static const char* SHARED_VERTEX_SOURCE = "#version 100\n"
    "\n"
    "attribute vec4 a_position;\n"
    "attribute vec4 a_color;\n"
    "attribute vec2 a_textureCoord;\n"
    "attribute vec2 a_localPosition;\n"
    "\n"
    "varying vec4 v_color;\n"
    "varying vec2 v_textureCoord;\n"
    "varying vec2 v_localPosition;\n"
    "\n"
    "uniform mat4 u_projection;\n"
    "\n"
    "void main() {\n"
    "   v_color         = a_color;\n"
    "   v_textureCoord  = a_textureCoord;\n"
    "   v_localPosition = a_localPosition;\n"
    "   gl_Position     = u_projection * a_position;\n"
    "}";

    static const char* QUAD_FRAGMENT_SOURCE = "#version 100\n"
    "\n"
    "precision mediump float;\n"
    "\n"
    "varying vec4 v_color;\n"
    "varying vec2 v_textureCoord;\n"
    "\n"
    "uniform sampler2D u_texture;\n"
    "\n"
    "void main() {\n"
    "   gl_FragColor = v_color * texture2D(u_texture, v_textureCoord);\n"
    "}";

    static const char* CIRCLE_FRAGMENT_SOURCE = "#version 100\n"
    "\n"
    "precision mediump float;\n"
    "\n"
    "varying vec4 v_color;\n"
    "varying vec2 v_localPosition;\n"
    "\n"
    "void main() {\n"
    "   float distance = length(v_localPosition);\n"
    "   float mask     = step(distance, 1.0);\n"
    "\n"
    "   if (mask == 0.0) {\n"
    "       discard;"
    "   }\n"
    "\n"
    "   gl_FragColor = v_color * vec4(vec3(mask), 1.0);\n"
    "}";

    static const char* LINE_FRAGMENT_SOURCE = "#version 100\n"
    "\n"
    "precision mediump float;\n"
    "\n"
    "varying vec4 v_color;\n"
    "\n"
    "void main() {\n"
    "   gl_FragColor = v_color;\n"
    "}";

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

    BatchRenderer::BatchRenderer() {

        // generate index buffer

        uint16_t* indexBuffer = new uint16_t[MAX_INDEX_COUNT];

		for (uint32_t i = 0, offset = 0; i < MAX_INDEX_COUNT; i += 6, offset += 4)
		{
			indexBuffer[i + 0] = offset + 0;
			indexBuffer[i + 1] = offset + 1;
			indexBuffer[i + 2] = offset + 2;

			indexBuffer[i + 3] = offset + 2;
			indexBuffer[i + 4] = offset + 3;
			indexBuffer[i + 5] = offset + 0;
		}

        // init buffers

        BufferLayout bufferLayout = {};

        bufferLayout.Push("a_position",      DataType::Float4);
        bufferLayout.Push("a_color",         DataType::Float4);
        bufferLayout.Push("a_textureCoord",  DataType::Float2);
        bufferLayout.Push("a_localPosition", DataType::Float2);

        m_batchVBO = MakeRef<VertexBuffer>(sizeof(Vertex) * MAX_VERTEX_COUNT, bufferLayout);
        m_batchIBO = MakeRef<IndexBuffer>(sizeof(uint16_t) * MAX_INDEX_COUNT, indexBuffer);

        uint32_t whitePixels[] = { 0xffffffff };
        m_whiteTexture         = MakeRef<Texture>(1, 1, &whitePixels);

        m_batchVertexBufferBase = MakeScope<Vertex[]>(MAX_VERTEX_COUNT);

        // init shaders

        m_quadShader   = MakeRef<Shader>(SHARED_VERTEX_SOURCE, QUAD_FRAGMENT_SOURCE,   bufferLayout);
        m_circleShader = MakeRef<Shader>(SHARED_VERTEX_SOURCE, CIRCLE_FRAGMENT_SOURCE, bufferLayout);
        m_lineShader   = MakeRef<Shader>(SHARED_VERTEX_SOURCE, LINE_FRAGMENT_SOURCE,   bufferLayout);
    }

    void BatchRenderer::Begin(const glm::mat4& projection) {
        m_projection   = projection;
        m_numDrawCalls = 0;

        glClear(GL_COLOR_BUFFER_BIT);
        StartBatch();
    }

    void BatchRenderer::End() {
        Flush();
    }

    void BatchRenderer::DrawRect(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
        DrawTexture(m_whiteTexture, MakeTransform(position, size, rotation), color);
    }

    void BatchRenderer::DrawRect(const glm::mat4& transform, const glm::vec4& color) {
        DrawTexture(m_whiteTexture, transform, color);
    }

    void BatchRenderer::DrawRectLines(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
        DrawRectLines(MakeTransform(position, size, rotation), color);
    }

    void BatchRenderer::DrawRectLines(const glm::mat4& transform, const glm::vec4& color) {
        glm::vec2 v0 = transform * QUAD_VERTEX_POSITIONS[0];
        glm::vec2 v1 = transform * QUAD_VERTEX_POSITIONS[1];
        glm::vec2 v2 = transform * QUAD_VERTEX_POSITIONS[2];
        glm::vec2 v3 = transform * QUAD_VERTEX_POSITIONS[3];

        DrawLine(v0, v1, color);
        DrawLine(v1, v2, color);
        DrawLine(v2, v3, color);
        DrawLine(v3, v0, color);
    }   

    void BatchRenderer::DrawTexture(Ref<Texture> texture, const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
        DrawTexture(texture, MakeTransform(position, size, rotation), color);
    }

    void BatchRenderer::DrawTexture(Ref<Texture> texture, const glm::mat4& transform, const glm::vec4& color) {
        if (RequiresFlushForSpace() || RequiresFlushForShape(BatchShape::Quad) || RequiresFlushForTexture(texture)) {
            Flush();
            StartBatch();
        }

        for (int i = 0; i < VERTICES_PER_QUAD; i++) {
            m_batchVertexBufferBase[m_batchVertexBufferOffset].position      = transform * QUAD_VERTEX_POSITIONS[i];
            m_batchVertexBufferBase[m_batchVertexBufferOffset].color         = color;
            m_batchVertexBufferBase[m_batchVertexBufferOffset].textureCoord  = QUAD_TEXTURE_COORDS[i];
            m_batchVertexBufferBase[m_batchVertexBufferOffset].localPosition = {};

            m_batchVertexBufferOffset++;
        }

        m_batchShape   = BatchShape::Quad;
        m_batchTexture = texture;

        m_batchVertexCount += VERTICES_PER_QUAD;
        m_batchIndexCount  += INDICES_PER_QUAD;
    }

    void BatchRenderer::DrawCircle(const glm::vec2& position, float radius, const glm::vec4& color) {
        DrawCircle(MakeTransform(position, { radius * 2.0f, radius * 2.0f }, 0.0f), color);
    }

    void BatchRenderer::DrawCircle(const glm::mat4& transform, const glm::vec4& color) {
        if (RequiresFlushForSpace() || RequiresFlushForShape(BatchShape::Circle)) {
            Flush();
            StartBatch();
        }

        for (int i = 0; i < VERTICES_PER_QUAD; i++) {
            m_batchVertexBufferBase[m_batchVertexBufferOffset].position      = transform * QUAD_VERTEX_POSITIONS[i];
            m_batchVertexBufferBase[m_batchVertexBufferOffset].color         = color;
            m_batchVertexBufferBase[m_batchVertexBufferOffset].textureCoord  = {};
            m_batchVertexBufferBase[m_batchVertexBufferOffset].localPosition = QUAD_VERTEX_POSITIONS[i] * 2.0f;

            m_batchVertexBufferOffset++;
        }

        m_batchShape = BatchShape::Circle;

        m_batchVertexCount += VERTICES_PER_QUAD;
        m_batchIndexCount  += INDICES_PER_QUAD;
    }

    void BatchRenderer::DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color) {
        if (RequiresFlushForSpace() || RequiresFlushForShape(BatchShape::Line)) {
            Flush();
            StartBatch();
        }

        m_batchVertexBufferBase[m_batchVertexBufferOffset].position      = glm::vec4(start, 0.0f, 1.0f);
        m_batchVertexBufferBase[m_batchVertexBufferOffset].color         = color;
        m_batchVertexBufferBase[m_batchVertexBufferOffset].textureCoord  = {};
        m_batchVertexBufferBase[m_batchVertexBufferOffset].localPosition = {};

        m_batchVertexBufferOffset++;

        m_batchVertexBufferBase[m_batchVertexBufferOffset].position      = glm::vec4(end, 0.0f, 1.0f);
        m_batchVertexBufferBase[m_batchVertexBufferOffset].color         = color;
        m_batchVertexBufferBase[m_batchVertexBufferOffset].textureCoord  = {};
        m_batchVertexBufferBase[m_batchVertexBufferOffset].localPosition = {};

        m_batchVertexBufferOffset++;

        m_batchShape = BatchShape::Line;
        m_batchVertexCount += VERTICES_PER_LINE;
    }
    
    void BatchRenderer::StartBatch() {
        m_batchShape    = BatchShape::None;
        m_batchTexture  = {}; 

        m_batchVertexCount = 0;
        m_batchIndexCount  = 0;

        m_batchVertexBufferOffset = 0;
    }

    void BatchRenderer::Flush() {
        if ( m_batchShape == BatchShape::None) {
            return;
        }

        size_t batchVertexBufferSize = sizeof(Vertex) * m_batchVertexCount;

        m_batchVBO->Use();
        m_batchVBO->SetData(batchVertexBufferSize, m_batchVertexBufferBase.get());
        m_batchVBO->BindAttributes();
        
        switch (m_batchShape) {
            case BatchShape::Quad: {
                m_quadShader->Use();

                glActiveTexture(GL_TEXTURE0);
                m_batchTexture->Use();
                m_quadShader->SetMat4("u_projection", m_projection);
                m_quadShader->SetInt("u_texture", 0);

                m_batchIBO->Use();
                glDrawElements(GL_TRIANGLES, m_batchIndexCount, GL_UNSIGNED_SHORT, NULL);

                break;
            }

            case BatchShape::Circle: {
                m_circleShader->Use();
                m_circleShader->SetMat4("u_projection", m_projection);

                m_batchIBO->Use();
                glDrawElements(GL_TRIANGLES, m_batchIndexCount, GL_UNSIGNED_SHORT, NULL);

                break;
            }

            case BatchShape::Line: {
                m_lineShader->Use();
                m_lineShader->SetMat4("u_projection", m_projection);

                glDrawArrays(GL_LINES, 0, m_batchVertexCount);

                break;
            }
        }

        m_numDrawCalls++;
    }

    bool BatchRenderer::RequiresFlushForSpace() {
        return m_batchVertexCount == MAX_VERTEX_COUNT;
    }

    bool BatchRenderer::RequiresFlushForShape(BatchShape shape) {
        return m_batchShape != BatchShape::None && m_batchShape != shape;
    }

    bool BatchRenderer::RequiresFlushForTexture(Ref<Texture> texture) {
        return m_batchShape != BatchShape::None && m_batchTexture != texture;
    }

}