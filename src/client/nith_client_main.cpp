#include "core/ecs/archetype.hpp"
#include "core/ecs/hierarchical_archetype.hpp"
#include "util/file.hpp"
#include "client/nith_client.hpp"
#include "client/window.hpp"
#include "core/const_string.hpp"
#include "client/graphic/mesh.hpp"

using BasicShader = nith::Shader<"color0", "color1", "projection">;

int main()
{
    // nith::NithClient nithClient;
    // nith::ShaderManager &shaderManager = nithClient.getShaderManager();

    // nith::Window &mainWindow = nithClient.getMainWindow();

    // mainWindow.open();

    // shaderManager.loadShaderFromFile("D:/github/nithcraft/resources/shaders/basic.vert",
    //                                  "D:/github/nithcraft/resources/shaders/basic.frag");

    // BasicShader basicShader = shaderManager.getShader<BasicShader>("basic");
    // basicShader.setUniform<"color0">(nith::vec3{1.0f, 1.0f, 0.5f});
    // basicShader.setUniform<"color1">(nith::vec3{0.0f, 1.0f, 0.5f});

    // float vertices[] = {
    //     0.5f, 0.5f, 0.0f, 0.5f, 1.0f, 0.5f,   // top right
    //     0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom right
    //     -0.5f, -0.5f, 0.0f, 0.5f, 1.0f, 0.5f, // bottom left
    //     -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.5f,  // top left
    // };
    // unsigned int indices[] = {
    //     // note that we start from 0!
    //     0, 1, 3, // first Triangle
    //     1, 2, 3  // second Triangle
    // };

    // nith::Mesh basicMesh;
    // basicMesh.setAttributes({
    //     {nith::Type::VEC3}, // position
    //     {nith::Type::VEC3}  // color
    // });

    // basicMesh.setVerticesData(vertices, sizeof(vertices));
    // basicMesh.setIndices(indices, 6);
    // basicMesh.setDrawCount(6);

    // while (!mainWindow.isClosed())
    // {
    //     mainWindow.beginLoop();

    //     basicShader.use();
    //     basicMesh.drawTriangles();

    //     mainWindow.endLoop();
    // }

    // mainWindow.closeImmediately();
    // glfwTerminate();

    struct TransformComponent
    {
        nith::f32 x, y, z;
    };

    struct MobComponent
    {
        nith::u32 health;
        bool isHungry;
    };

    using MobArchetype = nith::HierarchicalArchetype<
        TransformComponent,
        MobComponent>;

    MobArchetype mobArcheType;

    mobArcheType.addEntity(1);
    mobArcheType.addChildEntity(1, 10);

    mobArcheType.addEntity(2);

    mobArcheType.addChildEntity(10, 101);
    mobArcheType.addChildEntity(2, 20);

    mobArcheType.addEntity(3);
    mobArcheType.addChildEntity(20, 201);
    mobArcheType.addChildEntity(3, 30);

    mobArcheType.print();
    mobArcheType.removeEntity(1);
    mobArcheType.print();

    mobArcheType.removeEntity(2);
    mobArcheType.print();

    return 0;
}