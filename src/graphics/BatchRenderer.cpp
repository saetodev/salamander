#include "graphics/BatchRenderer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace sal {

    struct QuadVertex {
        glm::vec4 position;
        glm::vec4 color;
        glm::vec2 textureCoord;
    };

    struct CircleVertex {
        glm::vec4 worldPosition;
        glm::vec4 localPosition;
        glm::vec4 color;
    };

    struct LineVertex {
        glm::vec4 position;
        glm::vec4 color;
    };

    static const char* QUAD_VERTEX_SOURCE = "#version 100\n"
    "\n"
    "attribute vec4 a_position;\n"
    "attribute vec4 a_color;\n"
    "attribute vec2 a_textureCoord;\n"
    "\n"
    "varying vec4 v_color;\n"
    "varying vec2 v_textureCoord;\n"
    "\n"
    "uniform mat4 u_projection;\n"
    "\n"
    "void main() {\n"
    "   v_color        = a_color;\n"
    "   v_textureCoord = a_textureCoord;\n "
    "   gl_Position = u_projection * a_position;\n"
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

    static const char* CIRCLE_VERTEX_SOURCE = "#version 100\n"
    "\n"
    "attribute vec4 a_worldPosition;\n"
    "attribute vec4 a_localPosition;\n"
    "attribute vec4 a_color;\n"
    "\n"
    "varying vec4 v_localPosition;\n"
    "varying vec4 v_color;\n"
    "\n"
    "uniform mat4 u_projection;\n"
    "\n"
    "void main() {\n"
    "   v_localPosition = a_localPosition;\n"
    "   v_color         = a_color;\n"
    "   gl_Position     = u_projection * a_worldPosition;\n"
    "}";

    static const char* CIRCLE_FRAGMENT_SOURCE = "#version 100\n"
    "\n"
    "precision mediump float;\n"
    "\n"
    "varying vec4 v_localPosition;\n"
    "varying vec4 v_color;\n"
    "\n"
    "void main() {\n"
    "   float distance = length(v_localPosition.xy);\n"
    "   float mask     = step(distance, 1.0);\n"
    "\n"
    "   if (mask == 0.0) {\n"
    "       discard;"
    "   }\n"
    "\n"
    "   gl_FragColor = v_color * vec4(vec3(mask), 1.0);\n"
    "}";

    static const char* LINE_VERTEX_SOURCE = "#version 100\n"
    "\n"
    "attribute vec4 a_position;\n"
    "attribute vec4 a_color;\n"
    "\n"
    "varying vec4 v_color;\n"
    "\n"
    "uniform mat4 u_projection;\n"
    "\n"
    "void main() {\n"
    "   v_color     = a_color;\n"
    "   gl_Position = u_projection * a_position;\n"
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

        // init quads data

        BufferLayout quadBufferLayout = {};

        quadBufferLayout.Push("a_position", DataType::Float4);
        quadBufferLayout.Push("a_color", DataType::Float4);
        quadBufferLayout.Push("a_textureCoord", DataType::Float2);

        m_quadVBO = MakeRef<VertexBuffer>(sizeof(QuadVertex) * MAX_VERTEX_COUNT, quadBufferLayout);
        m_quadIBO = MakeRef<IndexBuffer>(sizeof(uint16_t) * MAX_INDEX_COUNT, indexBuffer);
    
        uint32_t whitePixels[] = { 0xffffffff };
        m_whiteTexture         = MakeRef<Texture>(1, 1, &whitePixels);
    
        m_quadVertexBufferBase = new QuadVertex[MAX_VERTEX_COUNT];
        m_quadVertexBufferPtr  = m_quadVertexBufferBase;

        // init circles data

        BufferLayout circleBufferLayout = {};

        circleBufferLayout.Push("a_worldPosition", DataType::Float4);
        circleBufferLayout.Push("a_localPosition", DataType::Float4);
        circleBufferLayout.Push("a_color", DataType::Float4);

        m_circleVBO = MakeRef<VertexBuffer>(sizeof(CircleVertex) * MAX_VERTEX_COUNT, circleBufferLayout);
        m_circleIBO = MakeRef<IndexBuffer>(sizeof(uint16_t) * MAX_INDEX_COUNT, indexBuffer);

        m_circleVertexBufferBase = new CircleVertex[MAX_VERTEX_COUNT];
        m_circleVertexBufferPtr  = m_circleVertexBufferBase;

        delete indexBuffer;

        // init lines data

        BufferLayout lineBufferLayout = {};

        lineBufferLayout.Push("a_position", DataType::Float4);
        lineBufferLayout.Push("a_color", DataType::Float4);

        m_lineVBO = MakeRef<VertexBuffer>(sizeof(LineVertex) * MAX_VERTEX_COUNT, lineBufferLayout);

        m_lineVertexBufferBase = new LineVertex[MAX_VERTEX_COUNT];
        m_lineVertexBufferPtr  = m_lineVertexBufferBase;

        // init shaders

        m_quadShader   = MakeRef<Shader>(QUAD_VERTEX_SOURCE, QUAD_FRAGMENT_SOURCE, quadBufferLayout);
        m_circleShader = MakeRef<Shader>(CIRCLE_VERTEX_SOURCE, CIRCLE_FRAGMENT_SOURCE, circleBufferLayout);
        m_lineShader   = MakeRef<Shader>(LINE_VERTEX_SOURCE, LINE_FRAGMENT_SOURCE, lineBufferLayout);
    }

    BatchRenderer::~BatchRenderer() {
        delete[] m_quadVertexBufferBase;
        delete[] m_circleVertexBufferBase;
        delete[] m_lineVertexBufferBase;
    }

    void BatchRenderer::Begin(const glm::mat4& projection) {
        m_projection = projection;

        glClear(GL_COLOR_BUFFER_BIT);
        StartBatch();
    }

    void BatchRenderer::End() {
        Flush();
    }

    void BatchRenderer::DrawRect(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
        glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
        glm::mat4 rotate    = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 scale     = glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

        glm::mat4 transform = translate * rotate * scale;

        DrawTexture(m_whiteTexture, transform, color);
    }

    void BatchRenderer::DrawRect(const glm::mat4& transform, const glm::vec4& color) {
        DrawTexture(m_whiteTexture, transform, color);
    }

    void BatchRenderer::DrawTexture(Ref<Texture> texture, const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
        glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
        glm::mat4 rotate    = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 scale     = glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

        glm::mat4 transform = translate * rotate * scale;

        DrawTexture(texture, transform, color);
    }

    void BatchRenderer::DrawTexture(Ref<Texture> texture, const glm::mat4& transform, const glm::vec4& color) {
        if (m_quadCount == MAX_QUAD_COUNT || RequiresFlushForShape(Shape::Quad) || RequiresFlushForTexture(texture)) {
            Flush();
            StartBatch();
        }

        for (int i = 0; i < VERTICES_PER_QUAD; i++) {
            m_quadVertexBufferPtr->position     = transform * QUAD_VERTEX_POSITIONS[i];
            m_quadVertexBufferPtr->color        = color;
            m_quadVertexBufferPtr->textureCoord = QUAD_TEXTURE_COORDS[i]; 

            m_quadVertexBufferPtr++;
        }

        m_batchShape   = Shape::Quad;
        m_batchTexture = texture;

        m_quadCount += 1;
    }

    void BatchRenderer::DrawCircle(const glm::vec2& position, float radius, const glm::vec4& color) {
        glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
        glm::mat4 scale     = glm::scale(glm::mat4(1.0f), glm::vec3(radius * 2.0f, radius * 2.0f, 1.0f));

        glm::mat4 transform = translate * scale;

        DrawCircle(transform, color);
    }

    void BatchRenderer::DrawCircle(const glm::mat4& transform, const glm::vec4& color) {
        if (m_circleCount == MAX_QUAD_COUNT || RequiresFlushForShape(Shape::Circle)) {
            Flush();
            StartBatch();
        }

        for (int i = 0; i < VERTICES_PER_QUAD; i++) {
            m_circleVertexBufferPtr->worldPosition = transform * QUAD_VERTEX_POSITIONS[i];
            m_circleVertexBufferPtr->localPosition = QUAD_VERTEX_POSITIONS[i] * 2.0f;
            m_circleVertexBufferPtr->color         = color;

            m_circleVertexBufferPtr++;
        }

        m_batchShape   = Shape::Circle;
        m_circleCount += 1;
    }

    //TODO: a quad is twice as many vertices as a line. so lineVertexBuffer is at most half full.
    void BatchRenderer::DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color) {
        if (m_lineCount == MAX_QUAD_COUNT || RequiresFlushForShape(Shape::Line)) {
            Flush();
            StartBatch();
        }

        m_lineVertexBufferPtr->position = glm::vec4(start, 0.0f, 1.0f);
        m_lineVertexBufferPtr->color    = color;

        m_lineVertexBufferPtr++;

        m_lineVertexBufferPtr->position = glm::vec4(end, 0.0f, 1.0f);
        m_lineVertexBufferPtr->color    = color;

        m_lineVertexBufferPtr++;

        m_batchShape = Shape::Line;
        m_lineCount += 1;
    }
    
    void BatchRenderer::StartBatch() {
        m_batchShape    = Shape::None;
        m_batchTexture  = {}; 

        m_quadCount = 0;
        m_quadVertexBufferPtr = m_quadVertexBufferBase;

        m_circleCount = 0;
        m_circleVertexBufferPtr = m_circleVertexBufferBase;

        m_lineCount = 0;
        m_lineVertexBufferPtr = m_lineVertexBufferBase;
    }

    void BatchRenderer::Flush() {
        if (m_batchShape == Shape::None) {
            return;
        }

        if (m_quadCount && m_batchShape == Shape::Quad) {
            size_t quadVertexBufferSize = (uintptr_t)m_quadVertexBufferPtr - (uintptr_t)m_quadVertexBufferBase;
            size_t quadIndexCount       = m_quadCount * INDICES_PER_QUAD;

            m_quadShader->Use();
            m_quadVBO->Use();
            m_quadIBO->Use();

            glActiveTexture(GL_TEXTURE0);
            m_whiteTexture->Use();

            m_quadShader->SetMat4("u_projection", m_projection);
            m_quadShader->SetInt("u_texture", 0);

            m_quadVBO->SetData(quadVertexBufferSize, m_quadVertexBufferBase);
            m_quadVBO->BindAttributes();

            glDrawElements(GL_TRIANGLES, quadIndexCount, GL_UNSIGNED_SHORT, NULL);
        }
        else if (m_circleCount && m_batchShape == Shape::Circle) {
            size_t circleVertexBufferSize = (uintptr_t)m_circleVertexBufferPtr - (uintptr_t)m_circleVertexBufferBase;
            size_t circleIndexCount       = m_circleCount * INDICES_PER_QUAD;

            m_circleShader->Use();
            m_circleVBO->Use();
            m_circleIBO->Use();

            m_circleShader->SetMat4("u_projection", m_projection);

            m_circleVBO->SetData(circleVertexBufferSize, m_circleVertexBufferBase);
            m_circleVBO->BindAttributes();

            glDrawElements(GL_TRIANGLES, circleIndexCount, GL_UNSIGNED_SHORT, NULL);
        }
        else if (m_lineCount && m_batchShape == Shape::Line) {
            size_t lineVertexBufferSize = (uintptr_t)m_lineVertexBufferPtr - (uintptr_t)m_lineVertexBufferBase;
            size_t lineVertexCount      = m_lineCount * VERTICES_PER_LINE;

            m_lineShader->Use();
            m_lineShader->SetMat4("u_projection", m_projection);

            m_lineVBO->Use();
            m_lineVBO->SetData(lineVertexBufferSize, m_lineVertexBufferBase);
            m_lineVBO->BindAttributes();

            glDrawArrays(GL_LINES, 0, lineVertexCount);
        }
    }

    bool BatchRenderer::RequiresFlushForShape(Shape shape) {
        return m_batchShape != Shape::None && m_batchShape != shape;
    }

    bool BatchRenderer::RequiresFlushForTexture(Ref<Texture> texture) {
        return m_batchShape != Shape::None && m_batchTexture != texture;
    }

}