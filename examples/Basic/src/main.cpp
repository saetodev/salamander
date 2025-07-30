#include "Salamander.h"

class Basic : public sal::App
{
public:
    void Init() override
    {
        m_camera = sal::Camera(-m_window->Width() * 0.5f, m_window->Width() * 0.5f, -m_window->Height() * 0.5f, m_window->Height() * 0.5f);
    }

    void Shutdown() override
    {

    }

    void Update(float delta) override
    {
        sal::RenderAPI::Clear(glm::vec4(0.2f, 0.2f, 0.4f, 1.0f));

        m_renderer->Begin(m_camera);

        m_renderer->DrawRect({0, 0}, {100, 100}, 0.0f, {1, 0, 0, 1});

        m_renderer->DrawRectLines({150, 0}, {100, 100}, 45.0f, {0, 1, 0, 1});

        m_renderer->DrawCircle({ -150, 0 }, 50, { 0, 0, 1, 1 });

        m_renderer->End();
    }
private:
    sal::Camera m_camera;
};

int main()
{
    Basic().Run();
}