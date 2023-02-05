#include "game.hpp"
#include "application.hpp"

namespace nith
{
    void GameInput::tick()
    {
        auto& window = Application::Get().getMainWindow();

        forward  = window.isKeyPressed(Keycode::W);
        backward = window.isKeyPressed(Keycode::S);
        left     = window.isKeyPressed(Keycode::A);
        right    = window.isKeyPressed(Keycode::D);
        up       = window.isKeyPressed(Keycode::SPACE);
        down     = window.isKeyPressed(Keycode::LEFT_SHIFT);
        
        mouseDelta = window.getDeltaMousePos();

        isMoved = forward || backward || left || right || up || down;
        isMouseMoved = glm::length(mouseDelta) > 0.1f;
    }

    Game* Game::s_game = nullptr;

    Game::Game():
        m_camera(Application::Get().getMainWindow().getAspect(), glm::radians(50.0f), 0.2f, 1000.0f)
    {
        s_game = this;
        m_world.loadChunk({ 0, 0, 0 });
        m_world.loadChunk({ 1, 0, 0 });
        m_world.loadChunk({ -1, 0, 0 });
        //m_world.loadChunk({ 0, 1, 0 });
        //m_world.loadChunk({ 0, -1, 0 });
        //m_world.loadChunk({ 0, 0, 1 });
        //m_world.loadChunk({ 0, 0, -1 });

    }

    Camera& Game::getCamera()
    {
        return m_camera;
    }

    void Game::tick(const f32& deltaTime)
    {
        m_input.tick();

        if (!Application::Get().getMainWindow().isEnableCursor())
        {
            if (m_input.isMouseMoved || m_input.isMoved)
            {
                auto& transform = m_camera.getTransform();

                if (m_input.isMoved)
                {
                    vec3 playerDir =
                        f32(m_input.right - m_input.left) * m_camera.getRight() +
                        f32(m_input.up - m_input.down) * m_camera.getUp() +
                        f32(m_input.forward - m_input.backward) * m_camera.getForward();

                    if (glm::length(playerDir) > 0.00001f)
                    {
                        playerDir = glm::normalize(playerDir);
                        transform.move(playerDir * 0.1f);
                    }
                }

                if (m_input.isMouseMoved)
                {
                    vec3 rotation = transform.getRotation();
                    rotation -= vec3({ m_input.mouseDelta.y, m_input.mouseDelta.x, 0 }) * 0.001f;

                    rotation.x = std::max(rotation.x, -f32(pi - 0.1f) / 2);
                    rotation.x = std::min(rotation.x, +f32(pi - 0.1f) / 2);

                    transform.setRotation(rotation);
                }

                m_camera.updateView();
            }
        }
    }

    Game& Game::Get()
    {
        return *s_game;
    }

    Game::~Game()
    {
        s_game = nullptr;
    }

    void Game::render(const f32& deltaTime)
    {
        m_world.render(deltaTime);
    }

}
