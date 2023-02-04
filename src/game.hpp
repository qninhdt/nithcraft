#pragma once

#include "camera.hpp"
#include "world/world.hpp"

namespace nith
{
    struct GameInput
    {
        bool forward = false;
        bool backward = false;
        bool up = false;
        bool down = false;
        bool right = false;
        bool left = false;

        vec2 mouseDelta;

        bool isMoved = false;
        bool isMouseMoved = false;

        void tick();
    };

    class Game
    {
    public:

        Game();

        Camera& getCamera();

        void tick(const f32& deltaTime);

        void render(const f32& deltaTime);
        
        static Game& Get();

        ~Game();

    private:

        static Game* s_game;

        World m_world;
        Camera m_camera;
        GameInput m_input;
    };
}