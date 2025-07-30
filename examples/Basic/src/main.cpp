#include "Salamander.h"

#include <iostream>

class Basic : public sal::App
{
public:
    void Init() override
    {
        sal::Window& window = App::GetWindow();

        m_camera = sal::Camera(-window.Width() * 0.5f, window.Width() * 0.5f, -window.Height() * 0.5f, window.Height() * 0.5f);
    }

    void Shutdown() override
    {

    }

    void Update(float delta) override
    {
        sal::BatchRenderer& renderer = App::GetRenderer();

        sal::RenderAPI::Clear(glm::vec4(0.2f, 0.2f, 0.4f, 1.0f));

        renderer.Begin(m_camera);

        renderer.DrawRect({0, 0}, {100, 100}, 0.0f, {1, 0, 0, 1});

        renderer.DrawRectLines({150, 0}, {100, 100}, 45.0f, {0, 1, 0, 1});

        renderer.DrawCircle({ -150, 0 }, 50, { 0, 0, 1, 1 });

        renderer.End();
    }
private:
    sal::Camera m_camera;
};

int main()
{
    Basic().Run();
}