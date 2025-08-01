#include "Salamander.h"

#include <iostream>

class SpaceInvaders : public sal::App
{
public:
    using sal::App::App;

    void Init() override
    {
        sal::Window window = sal::App::GetWindow();

        float wHalfWidth  = window.Width() * 0.5f;
        float wHalfHeight = window.Height() * 0.5f; 

        m_camera = sal::Camera(-wHalfWidth, wHalfWidth, -wHalfHeight, wHalfHeight);

        m_playerPosition = glm::vec2(0.0f, -wHalfHeight + 32.0f);
        m_playerSize     = glm::vec2(32.0f, 16.0f);

        m_sound = sal::Sound::Load(sal::App::GetAudio(), "assets/start-level.wav");
        m_music = sal::Sound::Load(sal::App::GetAudio(), "assets/meet-the-princess.wav");

        m_music->Play();
    } 

    void Shutdown() override
    {
        
    }

    void Update(float delta)
    {
        sal::Input input = sal::App::GetInput();

        if (input.KeyPressed(' '))
        {
            m_sound->Play();
        }

        float xvel = (input.KeyDown('D') - input.KeyDown('A')) * 200.0f;

        m_playerPosition.x += xvel * delta;

        Render(sal::App::GetRenderer());
    }

    void Render(sal::BatchRenderer& renderer)
    {
        sal::RenderAPI::Clear(sal::Color::BLACK);
    
        renderer.Begin(m_camera);

        renderer.DrawRect(m_playerPosition, m_playerSize, 0.0f, sal::Color::RED);

        renderer.End();
    }
private:
    sal::Camera m_camera;

    glm::vec2 m_playerPosition = {};
    glm::vec2 m_playerSize     = {};

    sal::Ref<sal::Sound> m_sound = {};
    sal::Ref<sal::Sound> m_music = {};
};

int main()
{
    sal::Settings settings = {
        .windowWidth  = 720,
        .windowHeight = 480,
        .windowTitle  = "SPACE INVADERS",
    };

    SpaceInvaders(settings).Run();
}