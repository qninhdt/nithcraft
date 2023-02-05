#include "application.hpp"
#include "window.hpp"
#include "core/const_string.hpp"
#include "graphic/mesh.hpp"
#include "core/bit_storage.hpp"
#include "core/palette.hpp"
#include "world/block/block.hpp"
#include "core/ecs/hierarchical_archetype.hpp"
#include "world/block/nature/dirt_block.hpp"
#include "world/block/nature/air_block.hpp"
#include "game.hpp"
#include "core/ecs/system.hpp"
#include "debug_layer.hpp"
#include "stb_image.h"

using namespace nith;

int main()
{
    Application nithClient;
    ShaderManager &shaderManager = nithClient.getShaderManager();

    Window& mainWindow = nithClient.getMainWindow();
    mainWindow.open();
    mainWindow.toggleCursor();

    nithClient.loadResources();

    //glEnable(GL_MULTISAMPLE);

    DebugLayer debugLayer(mainWindow);
    
    Mesh::GenerateGlobalIBO();

    const GLubyte* renderer = glGetString(GL_RENDERER);
    std::cout << renderer << '\n';

    Game game;
    Camera& camera = game.getCamera();

    camera.getTransform().setPosition({ 16, 16, -16 });
    camera.updateView();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    mainWindow.onKeyPressed(
        [&](const Keycode& key, const bool& isRepeated)
        {
            if (key == Keycode::F1 && !isRepeated)
            mainWindow.toggleCursor();
        }
    );

    while (!mainWindow.isClosed())
    {
        if (mainWindow.isKeyPressed(Keycode::ESCAPE))
            mainWindow.close();;

        game.tick(0);

        mainWindow.beginLoop();

        game.render(0);

        debugLayer.render();

        mainWindow.endLoop();
    }

    mainWindow.closeImmediately();
    glfwTerminate();

    return 0;
}