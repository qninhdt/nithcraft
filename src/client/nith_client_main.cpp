#include "core/ecs/archetype.hpp"
#include "core/ecs/hierarchical_archetype.hpp"
#include "util/file.hpp"
#include "client/nith_client.hpp"
#include "client/window.hpp"
#include "core/const_string.hpp"
#include "client/graphic/mesh.hpp"
#include "core/ecs/id_stack.hpp"

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
        std::string name;
    };

    using MobArchetype = nith::ecs::HierarchicalArchetype<
        TransformComponent,
        MobComponent>;

    MobArchetype mobArcheType;

    mobArcheType.addEntity(1);
    mobArcheType.addChildEntity(1, 10);
    mobArcheType.addEntity(2);
    mobArcheType.addChildEntity(1, 11);

    mobArcheType.addEntity(3);

    mobArcheType.setComponent(2, MobComponent{"the 2"});
    mobArcheType.setComponent(3, MobComponent{"the 3"});
    mobArcheType.setComponent(10, MobComponent{"the 10"});
    mobArcheType.setComponent(11, MobComponent{"the 11"});
    mobArcheType.setComponent(1, MobComponent{"the 1"});

    mobArcheType.print();

    mobArcheType.each<MobComponent>(
        [](MobComponent &mob)
        {
            std::cout << mob.name << '\n';
        });

    mobArcheType.removeEntity(1);

    mobArcheType.print();

    mobArcheType.each<MobComponent>(
        [](MobComponent &mob)
        {
            std::cout << mob.name << '\n';
        });
    return 0;
}