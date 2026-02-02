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
    }

    void Shutdown() {

    }

    void Update(float delta) {
        std::cout << "delta: " << delta * 1000.0f << " ms\n";
        std::cout << "count: " << m_entities.size() << "\n\n";

        sal::Input& input = sal::App::GetInput();

        if (input.MouseButtonDown(0)) {
            glm::vec2 mousePos = input.MousePosition();
           
            float dx = (std::rand() % 3) - 1;
            float dy = (std::rand() % 3) - 1;

            float r = (std::rand() % 256) / 255.0f;
            float g = (std::rand() % 256) / 255.0f;
            float b = (std::rand() % 256) / 255.0f;

            m_entities.emplace_back(mousePos, glm::vec2(100.0f * dx, 100.0f * dy), glm::vec2(32.0f, 32.0f), glm::vec4(r, g, b, 1.0f));
        }

        UpdateEntities(delta);

        sal::RenderAPI::Clear(sal::Color::BLACK);
        RenderEntities();
    }
private:
    void UpdateEntities(float delta) {
        sal::Window& window = sal::App::GetWindow();

        for (Entity& entity : m_entities) {
            entity.position += entity.velocity * delta;

            glm::vec2 min = entity.position - entity.size * 0.5f;
            glm::vec2 max = entity.position + entity.size * 0.5f;

            if (min.x < 0.0f || max.x >= window.Width()) {
                entity.velocity.x *= -1.0f;
            }

            if (min.y < 0.0f || max.y >= window.Height()) {
                entity.velocity.y *= -1.0f;
            }
        }
    }

    void RenderEntities() {
        sal::BatchRenderer& renderer = sal::App::GetRenderer();

        renderer.Begin(m_camera);

        for (const Entity& entity : m_entities) {
            renderer.DrawRect(entity.position, entity.size, 0.0f, entity.color);
        }

        renderer.End();
    }
private:
    sal::Camera m_camera;

    std::vector<Entity> m_entities;
};

int main() {
    BunnyMark().Run();
}
