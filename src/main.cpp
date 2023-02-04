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

using namespace nith;

using BasicShader = Shader<"projection_view">;

int main()
{
    Application nithClient;
    ShaderManager &shaderManager = nithClient.getShaderManager();

    Window& mainWindow = nithClient.getMainWindow();
    mainWindow.open();

    Game game;
    Camera& camera = game.getCamera();

    camera.getTransform().setPosition({ 0, 0, -1 });
    camera.updateView();

    glfwSetInputMode(mainWindow.getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    shaderManager.loadShaderFromFile("D:/github/nithcraft/resources/shaders/basic.vert",
                                     "D:/github/nithcraft/resources/shaders/basic.frag");

    BasicShader basicShader = shaderManager.getShader<BasicShader>("basic");

    float vertices[] = {
        0.5f, 0.5f, 0.0f, 0.5f, 1.0f, 0.5f,   // top right
        0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.5f, 1.0f, 0.5f, // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.5f,  // top left
    };
    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };

    Mesh::GenerateGlobalIBo();

    Mesh basicMesh;
    basicMesh.setAttributes({
        {Type::VEC3}, // position
        {Type::VEC3}  // color
    });

    basicMesh.setVerticesData(vertices, sizeof(vertices));
    basicMesh.setIndices(indices, 6);
    basicMesh.setDrawCount(6);

    game.m_world.loadChunk({ 0, 0, 0 });

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    while (!mainWindow.isClosed())
    {
        if (mainWindow.isKeyPressed(Keycode::ESCAPE))
            mainWindow.close();


        basicShader.setUniform<"projection_view">(camera.getProjectionView());

        mainWindow.beginLoop();

        basicShader.use();
        game.tick();

        basicMesh.drawTriangles();

        mainWindow.endLoop();
    }

    mainWindow.closeImmediately();
    glfwTerminate();

    return 0;
}