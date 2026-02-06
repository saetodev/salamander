#include "Salamander.h"

struct Entity {
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 size;
    glm::vec4 color;
};

class BunnyMark : public sal::App {
public:
    void Init() {
        sal::Window& window = sal::App::GetWindow();
        m_camera = sal::Camera(0.0f, window.Width(), window.Height(), 0.0f);

        m_texture = LoadTexture("raybunny.png");
    }

    void Shutdown() {
    }

    void Update(float delta) {
        std::cout << "delta:   " << deltaAverage * 1000.0f << " ms\n";
        std::cout << "count:   " << m_entities.size() << "\n";
        std::cout << "batches: " << sal::App::GetRenderer().NumDrawCalls() << "\n\n";

        sal::Input& input = sal::App::GetInput();

        if (deltaAverage < (1.0f / 61.0f)) {
            glm::vec2 mousePos = input.MousePosition();
           
            for (int i = 0; i < 50; i++) {
                float angle = ((float)rand() / RAND_MAX) * 2.0 * M_PI;

                float dx = cos(angle);
                float dy = sin(angle);

                float r = (std::rand() % 256) / 255.0f;
                float g = (std::rand() % 256) / 255.0f;
                float b = (std::rand() % 256) / 255.0f;

                m_entities.emplace_back(glm::vec2(320, 240), glm::vec2(100.0f * dx, 100.0f * dy), glm::vec2(32.0f, 32.0f), glm::vec4(r, g, b, 1.0f));
            }
        }

        UpdateEntities(delta);

        sal::gpu::clear(0.0f, 0.0f, 0.0f, 1.0f);
        
        RenderEntities();

        timeAccum  += delta;
        deltaAccum += delta;
        frameCount += 1;

        if (timeAccum >= 1.0f) {
            deltaAverage = deltaAccum / frameCount;

            timeAccum -= 1.0f;
            deltaAccum = 0.0f;
            frameCount = 0;
        }
    }
private:
    void UpdateEntities(float delta) {
        sal::Window& window = sal::App::GetWindow();

        for (Entity& entity : m_entities) {
            entity.position += entity.velocity * delta;

            glm::vec2 min = entity.position - entity.size * 0.5f;
            glm::vec2 max = entity.position + entity.size * 0.5f;

            if (min.x < 0.0f) {
                entity.velocity.x *= -1.0f;
                entity.position.x = entity.size.x * 0.5f;
            }
            else if (max.x > window.Width()) {
                entity.velocity.x *= -1.0f;
                entity.position.x = window.Width() - entity.size.x * 0.5f;
            }

            if (min.y < 0.0f) {
                entity.velocity.y *= -1.0f;
                entity.position.y = entity.size.y * 0.5f;
            }
            else if (max.y > window.Height()) {
                entity.velocity.y *= -1.0f;
                entity.position.y = window.Height() - entity.size.y * 0.5f;
            }
        }
    }

    void RenderEntities() {
        sal::Renderer2D& renderer = sal::App::GetRenderer();

        renderer.Begin(m_camera);

        for (const Entity& entity : m_entities) {
            renderer.DrawTexture(m_texture, entity.position, entity.size, 0.0f, entity.color);
        }

        renderer.End();
    }
private:
    sal::Camera            m_camera  = {};
    sal::Ref<sal::Texture> m_texture = {};
    std::vector<Entity>    m_entities;

    float deltaAverage  = 0.0f;
    float deltaAccum    = 0.0f;
    float timeAccum     = 0.0f;
    int   frameCount    = 0;
};

int main() {
    BunnyMark().Run();
}
