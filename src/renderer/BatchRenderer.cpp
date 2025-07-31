#include "renderer/Buffer.h"
#include "renderer/BatchRenderer.h"
#include "renderer/RenderAPI.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"

#include <glad/glad.h>

namespace sal
{

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
    "uniform mat4 u_view;\n"
    "\n"
    "void main() {\n"
    "   v_color         = a_color;\n"
    "   v_textureCoord  = a_textureCoord;\n"
    "   v_localPosition = a_localPosition;\n"
    "   gl_Position     = u_projection * u_view * a_position;\n"
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

    void BatchRenderer::Init() {

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

        m_bufferLayout.Push("a_position",      EShaderDataType::FLOAT4);
        m_bufferLayout.Push("a_color",         EShaderDataType::FLOAT4);
        m_bufferLayout.Push("a_textureCoord",  EShaderDataType::FLOAT2);
        m_bufferLayout.Push("a_localPosition", EShaderDataType::FLOAT2);

        m_batchVBO = MakeRef<Buffer>(EBufferTarget::VERTEX, sizeof(Vertex) * MAX_VERTEX_COUNT, EBufferUsage::DYNAMIC);
        m_batchIBO = MakeRef<Buffer>(EBufferTarget::INDEX, sizeof(uint16_t) * MAX_INDEX_COUNT, indexBuffer, EBufferUsage::STATIC);

        uint32_t whitePixels[] = { 0xffffffff };
        m_whiteTexture         = MakeRef<Texture>(1, 1, &whitePixels);

        m_batchVertexBufferBase = new Vertex[MAX_VERTEX_COUNT];
        m_batchVertexBufferPtr  = m_batchVertexBufferBase;

        // init shaders

        m_quadShader   = MakeRef<Shader>(SHARED_VERTEX_SOURCE, QUAD_FRAGMENT_SOURCE,   m_bufferLayout);
        m_circleShader = MakeRef<Shader>(SHARED_VERTEX_SOURCE, CIRCLE_FRAGMENT_SOURCE, m_bufferLayout);
        m_lineShader   = MakeRef<Shader>(SHARED_VERTEX_SOURCE, LINE_FRAGMENT_SOURCE,   m_bufferLayout);

        // cleanup

        delete[] indexBuffer;
    }

    void BatchRenderer::Begin(const Camera& camera) {
        m_camera = camera;
        m_numDrawCalls = 0;

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

    void BatchRenderer::DrawTexture(const Ref<Texture>& texture, const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
        DrawTexture(texture, MakeTransform(position, size, rotation), color);
    }

    void BatchRenderer::DrawTexture(const Ref<Texture>& texture, const glm::mat4& transform, const glm::vec4& color) {
        DrawTexture(texture, { 0.0f, 0.0f, texture->Width(), texture->Height() }, transform, color);
    }

    void BatchRenderer::DrawTexture(const Ref<Texture>& texture, const glm::vec4& source, const glm::mat4& transform, const glm::vec4& color, const glm::vec2& tilingFactor) {
        if (RequiresFlushForSpace() || RequiresFlushForMode(BatchMode::Quad) || RequiresFlushForTexture(texture)) {
            Flush();
            StartBatch();
        }

        glm::vec4 textureSize  = glm::vec4(texture->Width(), texture->Height(), texture->Width(), texture->Height());
        glm::vec4 scaledSource = source / textureSize;
        glm::mat4 uvTransform  = MakeTransform(glm::vec2(scaledSource.x, scaledSource.y), glm::vec2(scaledSource.z, scaledSource.w), 0.0f);

        for (int i = 0; i < VERTICES_PER_QUAD; i++) {
            m_batchVertexBufferPtr->position      = transform * QUAD_VERTEX_POSITIONS[i];
            m_batchVertexBufferPtr->color         = color;
            m_batchVertexBufferPtr->textureCoord  = glm::vec2(uvTransform * glm::vec4(QUAD_TEXTURE_COORDS[i], 0.0f, 1.0f)) * tilingFactor;
            m_batchVertexBufferPtr->localPosition = {};

            m_batchVertexBufferPtr++;
        }

        m_batchMode    = BatchMode::Quad;
        m_batchTexture = texture;

        m_batchVertexCount += VERTICES_PER_QUAD;
        m_batchIndexCount  += INDICES_PER_QUAD;
    }

    void BatchRenderer::DrawCircle(const glm::vec2& position, float radius, const glm::vec4& color) {
        if (RequiresFlushForSpace() || RequiresFlushForMode(BatchMode::Circle)) {
            Flush();
            StartBatch();
        }

        glm::mat4 transform = MakeTransform(position, glm::vec2(radius) * 2.0f, 0.0f);

        for (int i = 0; i < VERTICES_PER_QUAD; i++) {
            m_batchVertexBufferPtr->position      = transform * QUAD_VERTEX_POSITIONS[i];
            m_batchVertexBufferPtr->color         = color;
            m_batchVertexBufferPtr->textureCoord  = {};
            m_batchVertexBufferPtr->localPosition = QUAD_VERTEX_POSITIONS[i] * 2.0f;

            m_batchVertexBufferPtr++;
        }

        m_batchMode = BatchMode::Circle;

        m_batchVertexCount += VERTICES_PER_QUAD;
        m_batchIndexCount  += INDICES_PER_QUAD;
    }

    void BatchRenderer::DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color) {
        if (RequiresFlushForSpace() || RequiresFlushForMode(BatchMode::Line)) {
            Flush();
            StartBatch();
        }

        m_batchVertexBufferPtr->position      = glm::vec4(start, 0.0f, 1.0f);
        m_batchVertexBufferPtr->color         = color;
        m_batchVertexBufferPtr->textureCoord  = {};
        m_batchVertexBufferPtr->localPosition = {};

        m_batchVertexBufferPtr++;

        m_batchVertexBufferPtr->position      = glm::vec4(end, 0.0f, 1.0f);
        m_batchVertexBufferPtr->color         = color;
        m_batchVertexBufferPtr->textureCoord  = {};
        m_batchVertexBufferPtr->localPosition = {};

        m_batchVertexBufferPtr++;

        m_batchMode = BatchMode::Line;
        m_batchVertexCount += VERTICES_PER_LINE;
    }
    
    void BatchRenderer::StartBatch() {
        m_batchMode    = BatchMode::None;
        m_batchTexture = {}; 

        m_batchVertexCount = 0;
        m_batchIndexCount  = 0;

        m_batchVertexBufferPtr = m_batchVertexBufferBase;
    }

    void BatchRenderer::Flush() {
        if (m_batchMode == BatchMode::None) {
            return;
        }

        m_camera.RecalculateViewMatrix();

        size_t batchVertexBufferSize = sizeof(Vertex) * m_batchVertexCount;

        m_batchVBO->Use();
        m_batchIBO->Use();
        m_batchVBO->SetData(batchVertexBufferSize, m_batchVertexBufferBase);
        
        switch (m_batchMode) {
            case BatchMode::Quad: {
                m_quadShader->Use();

                RenderAPI::BindTextureUnit(m_batchTexture, 0);
                RenderAPI::BindVertexAttribs(m_bufferLayout);

                m_quadShader->SetMat4("u_projection", m_camera.ProjectionMatrix());
                m_quadShader->SetMat4("u_view", m_camera.ViewMatrix());
                m_quadShader->SetInt("u_texture", 0);

                RenderAPI::DrawBufferIndexed(Primitive::Triangles, m_batchIndexCount);

                break;
            }

            case BatchMode::Circle: {
                m_circleShader->Use();

                RenderAPI::BindVertexAttribs(m_bufferLayout);

                m_circleShader->SetMat4("u_projection", m_camera.ProjectionMatrix());
                m_circleShader->SetMat4("u_view", m_camera.ViewMatrix());

                m_batchIBO->Use();

                RenderAPI::DrawBufferIndexed(Primitive::Triangles, m_batchIndexCount);

                break;
            }

            case BatchMode::Line: {
                m_lineShader->Use();

                RenderAPI::BindVertexAttribs(m_bufferLayout);

                m_lineShader->SetMat4("u_projection", m_camera.ProjectionMatrix());
                m_lineShader->SetMat4("u_view", m_camera.ViewMatrix());

                RenderAPI::DrawBuffer(Primitive::Lines, m_batchVertexCount);

                break;
            }
        }

        m_numDrawCalls++;
    }

    bool BatchRenderer::RequiresFlushForSpace() {
        return m_batchVertexCount == MAX_VERTEX_COUNT;
    }

    bool BatchRenderer::RequiresFlushForMode(BatchMode mode) {
        return m_batchMode != BatchMode::None && m_batchMode != mode;
    }

    bool BatchRenderer::RequiresFlushForTexture(const Ref<Texture>& texture) {
        return m_batchMode != BatchMode::None && m_batchTexture != texture;
    }

}