#include "graphics/Renderer2D.h"

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

    static constexpr int MAX_QUAD_COUNT   = 10000;
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

    void Renderer2D::Init() {

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

        // init vertex layout

        m_layout = {
            .count      = 4,
            .size       = sizeof(Vertex),
            .attributes = new gpu::VertexAttribute[4],
        };

        m_layout.attributes[0] = {
            .format = gpu::VertexFormat::FLOAT4,
            .name   = "a_position",
        };

        m_layout.attributes[1] = {
            .format = gpu::VertexFormat::FLOAT4,
            .name   = "a_color",
        };

        m_layout.attributes[2] = {
            .format = gpu::VertexFormat::FLOAT2,
            .name   = "a_textureCoord",
        };

        m_layout.attributes[3] = {
            .format = gpu::VertexFormat::FLOAT2,
            .name   = "a_localPosition",
        };
        
        // init buffers

        gpu::BufferDesc vboDesc = {
            .type  = gpu::BufferType::VERTEX,
            .usage = gpu::BufferUsage::DYNAMIC,
            .size  = sizeof(Vertex) * MAX_VERTEX_COUNT,
        };

        gpu::BufferDesc iboDesc = {
            .type  = gpu::BufferType::INDEX,
            .usage = gpu::BufferUsage::STATIC,
            .size  = sizeof(uint16_t) * MAX_INDEX_COUNT, 
            .data  = indexBuffer,
        };

        m_batchVBO = gpu::createBuffer(vboDesc);
        m_batchIBO = gpu::createBuffer(iboDesc);

        m_vertexBufferBase = new Vertex[MAX_VERTEX_COUNT];
        m_vertexBufferPtr  = m_vertexBufferBase;

        // init texture

        uint32_t whitePixels[] = { 0xffffffff };

        gpu::TextureDesc texDesc = {
            .filter = gpu::TextureFilter::NEAREST,
            .wrap   = gpu::TextureWrap::CLAMP,
            .format = gpu::PixelFormat::RGBA,
            .width  = 1,
            .height = 1,
            .pixels = whitePixels,
        };

        m_whiteTexture = gpu::createTexture(texDesc);

        // init shaders

        gpu::ShaderDesc quadShaderDesc = {
            .vertexSource   = SHARED_VERTEX_SOURCE,
            .fragmentSource = QUAD_FRAGMENT_SOURCE,
            .layout         = m_layout,
        };

        gpu::ShaderDesc circleShaderDesc = {
            .vertexSource   = SHARED_VERTEX_SOURCE,
            .fragmentSource = CIRCLE_FRAGMENT_SOURCE,
            .layout         = m_layout,
        };

        gpu::ShaderDesc lineShaderDesc = {
            .vertexSource   = SHARED_VERTEX_SOURCE,
            .fragmentSource = LINE_FRAGMENT_SOURCE,
            .layout         = m_layout,
        };

        m_quadShader   = gpu::createShader(quadShaderDesc);
        m_circleShader = gpu::createShader(circleShaderDesc);
        m_lineShader   = gpu::createShader(lineShaderDesc);

        // cleanup

        delete[] indexBuffer;
    }

    void Renderer2D::Shutdown()
    {
        gpu::destroyBuffer(m_batchVBO);
        gpu::destroyBuffer(m_batchIBO);

        gpu::destroyTexture(m_whiteTexture);

        gpu::destroyShader(m_quadShader);
        gpu::destroyShader(m_circleShader);
        gpu::destroyShader(m_lineShader);

        delete[] m_vertexBufferBase;
    }

    void Renderer2D::Begin(const Camera& camera) {
        m_camera = camera;
        m_numDrawCalls = 0;
            
        gpu::bind(gpu::BufferType::VERTEX, m_batchVBO);
        gpu::bind(m_layout);

        StartBatch();
    }

    void Renderer2D::End() {
        Flush();
    }

    void Renderer2D::DrawRect(glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color) {
        DrawTexture(m_whiteTexture, position, size, rotation, color);
    }

    void Renderer2D::DrawTexture(gpu::Texture texture, glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color) {
        if (RequiresFlushForSpace() || RequiresFlushForMode(BatchMode::Quad) || RequiresFlushForTexture(texture)) {
            Flush();
            StartBatch();
        }

        glm::mat4 transform(1.0f);

        transform = glm::translate(transform, glm::vec3(position, 0.0f));
        transform = glm::rotate(transform, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(size, 1.0f));

        for (int i = 0; i < VERTICES_PER_QUAD; i++) {
            m_vertexBufferPtr->position      = transform * QUAD_VERTEX_POSITIONS[i];
            m_vertexBufferPtr->color         = color;
            m_vertexBufferPtr->textureCoord  = QUAD_TEXTURE_COORDS[i];
            m_vertexBufferPtr->localPosition = {};

            m_vertexBufferPtr++;
        }

        m_batchMode    = BatchMode::Quad;
        m_batchTexture = texture;

        m_vertexCount += VERTICES_PER_QUAD;
        m_indexCount  += INDICES_PER_QUAD;
    }

    void Renderer2D::DrawCircle(glm::vec2 position, float radius, glm::vec4 color) {
        if (RequiresFlushForSpace() || RequiresFlushForMode(BatchMode::Circle)) {
            Flush();
            StartBatch();
        }

        glm::mat4 transform = MakeTransform(position, glm::vec2(radius) * 2.0f, 0.0f);

        for (int i = 0; i < VERTICES_PER_QUAD; i++) {
            m_vertexBufferPtr->position      = transform * QUAD_VERTEX_POSITIONS[i];
            m_vertexBufferPtr->color         = color;
            m_vertexBufferPtr->textureCoord  = {};
            m_vertexBufferPtr->localPosition = QUAD_VERTEX_POSITIONS[i] * 2.0f;

            m_vertexBufferPtr++;
        }

        m_batchMode = BatchMode::Circle;

        m_vertexCount += VERTICES_PER_QUAD;
        m_indexCount  += INDICES_PER_QUAD;
    }

    void Renderer2D::DrawLine(glm::vec2 start, glm::vec2 end, glm::vec4 color) {
        if (RequiresFlushForSpace() || RequiresFlushForMode(BatchMode::Line)) {
            Flush();
            StartBatch();
        }

        m_vertexBufferPtr->position      = glm::vec4(start, 0.0f, 1.0f);
        m_vertexBufferPtr->color         = color;
        m_vertexBufferPtr->textureCoord  = {};
        m_vertexBufferPtr->localPosition = {};

        m_vertexBufferPtr++;

        m_vertexBufferPtr->position      = glm::vec4(end, 0.0f, 1.0f);
        m_vertexBufferPtr->color         = color;
        m_vertexBufferPtr->textureCoord  = {};
        m_vertexBufferPtr->localPosition = {};

        m_vertexBufferPtr++;

        m_batchMode = BatchMode::Line;
        m_vertexCount += VERTICES_PER_LINE;
    }
    
    void Renderer2D::StartBatch() {
        m_batchMode    = BatchMode::None;
        m_batchTexture = {}; 

        m_vertexCount = 0;
        m_indexCount  = 0;

        m_vertexBufferPtr = m_vertexBufferBase;
    }

    void Renderer2D::Flush() {
        if (m_batchMode == BatchMode::None) {
            return;
        }

        m_camera.RecalculateViewMatrix();

        gpu::bind(gpu::BufferType::VERTEX, m_batchVBO);
        gpu::setBufferData(gpu::BufferType::VERTEX, m_batchVBO, sizeof(Vertex) * m_vertexCount, m_vertexBufferBase);
        
        gpu::bind(gpu::BufferType::INDEX, m_batchIBO);

        switch (m_batchMode) {
            case BatchMode::Quad: {
                gpu::bind(m_quadShader);
                gpu::bind(0, m_batchTexture);

                gpu::setShaderUniform(m_quadShader, "u_projection", m_camera.ProjectionMatrix());
                gpu::setShaderUniform(m_quadShader, "u_view", m_camera.ViewMatrix());
                gpu::setShaderUniform(m_quadShader, "u_texture", 0);

                gpu::drawPrimitivesIndexed(gpu::PrimitiveType::TRIANGLE_LIST, m_indexCount);

                break;
            }

            case BatchMode::Circle: {
                gpu::bind(m_circleShader);

                gpu::setShaderUniform(m_circleShader, "u_projection", m_camera.ProjectionMatrix());
                gpu::setShaderUniform(m_circleShader, "u_view", m_camera.ViewMatrix());

                gpu::drawPrimitivesIndexed(gpu::PrimitiveType::TRIANGLE_LIST, m_indexCount);

                break;
            }

            case BatchMode::Line: {
                gpu::bind(m_lineShader);

                gpu::setShaderUniform(m_lineShader, "u_projection", m_camera.ProjectionMatrix());
                gpu::setShaderUniform(m_lineShader, "u_view", m_camera.ViewMatrix());

                gpu::drawPrimitives(gpu::PrimitiveType::LINE_LIST, m_vertexCount);

                break;
            }
        }

        m_numDrawCalls++;
    }

    bool Renderer2D::RequiresFlushForSpace() {
        return m_vertexCount == MAX_VERTEX_COUNT;
    }

    bool Renderer2D::RequiresFlushForMode(BatchMode mode) {
        return m_batchMode != BatchMode::None && m_batchMode != mode;
    }

    bool Renderer2D::RequiresFlushForTexture(gpu::Texture texture) {
        return m_batchMode != BatchMode::None && m_batchTexture.id != texture.id;
    }

}