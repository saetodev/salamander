#include "Salamander.h"

using namespace sal;

static const char* vertexSource = "#version 100"
"\n"
"attribute vec2 a_position;\n"
"\n"
"void main() {\n"
"   gl_Position = vec4(a_position, 0.0, 1.0);\n"
"}";

static const char* fragmentSource = "#version 100"
"\n"
"precision mediump float;\n"
"\n"
"void main() {\n"
"   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
"}";

class Triangle : public App {
public:
    void Init() {
        m_layout = {
            .count      = 1,
            .size       = sizeof(float) * 2,
            .attributes = new gpu::VertexAttribute[1],
        };

        m_layout.attributes[0] = {
            .format = gpu::VertexFormat::FLOAT2,
            .name   = "a_position",
        };

        gpu::ShaderDesc shaderDesc = {
            .vertexSource   = vertexSource,
            .fragmentSource = fragmentSource,
            .layout         = m_layout,
        };

        m_shader = MakeRef<Shader>(shaderDesc);

        float vertexData[] = {
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.0f,  0.5f,
        };

        gpu::BufferDesc bufferDesc = {
            .type  = gpu::BufferType::VERTEX,
            .usage = gpu::BufferUsage::STATIC,
            .size  = sizeof(vertexData),
            .data  = vertexData,
        };

        m_vertexBuffer = MakeRef<VertexBuffer>(bufferDesc);
    }

    void Shutdown() {
        m_shader.reset();
        m_vertexBuffer.reset();
    }

    void Update(float delta) {
        gpu::bind(m_shader->handle());
        gpu::bind(gpu::BufferType::VERTEX, m_vertexBuffer->handle());
        gpu::bind(m_layout);

        gpu::clear(0.25f, 0.25f, 0.25f, 1.0f);
        gpu::drawPrimitives(gpu::PrimitiveType::TRIANGLE_LIST, 3);
    }
private:
    gpu::VertexLayout m_layout       = {};
    Ref<Shader>       m_shader       = {};
    Ref<VertexBuffer> m_vertexBuffer = {};
};

int main() {
    Triangle().Run();
}