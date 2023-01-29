#include "client/nith_client.hpp"
#include "client/window.hpp"
#include "core/const_string.hpp"
#include "client/graphic/mesh.hpp"
#include "core/bit_storage.hpp"
#include "core/palette.hpp"
#include "world/block/block.hpp"
#include "core/ecs/hierarchical_archetype.hpp"

using BasicShader = nith::Shader<"color0", "color1", "projection">;

// int main()
// {
//     nith::NithClient nithClient;
//     nith::ShaderManager &shaderManager = nithClient.getShaderManager();

//     nith::Window &mainWindow = nithClient.getMainWindow();

//     mainWindow.open();

//     shaderManager.loadShaderFromFile("D:/github/nithcraft/resources/shaders/basic.vert",
//                                      "D:/github/nithcraft/resources/shaders/basic.frag");

//     BasicShader basicShader = shaderManager.getShader<BasicShader>("basic");
//     basicShader.setUniform<"color0">(nith::vec3{1.0f, 1.0f, 0.5f});
//     basicShader.setUniform<"color1">(nith::vec3{0.0f, 1.0f, 0.5f});

//     float vertices[] = {
//         0.5f, 0.5f, 0.0f, 0.5f, 1.0f, 0.5f,   // top right
//         0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom right
//         -0.5f, -0.5f, 0.0f, 0.5f, 1.0f, 0.5f, // bottom left
//         -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.5f,  // top left
//     };
//     unsigned int indices[] = {
//         // note that we start from 0!
//         0, 1, 3, // first Triangle
//         1, 2, 3  // second Triangle
//     };

//     nith::Mesh basicMesh;
//     basicMesh.setAttributes({
//         {nith::Type::VEC3}, // position
//         {nith::Type::VEC3}  // color
//     });

//     basicMesh.setVerticesData(vertices, sizeof(vertices));
//     basicMesh.setIndices(indices, 6);
//     basicMesh.setDrawCount(6);

//     while (!mainWindow.isClosed())
//     {
//         mainWindow.beginLoop();

//         basicShader.use();
//         basicMesh.drawTriangles();

//         mainWindow.endLoop();
//     }

//     mainWindow.closeImmediately();
//     glfwTerminate();

//     nith::Palette<nith::u32, 64> palette;

//     palette.set(10, 100);
//     palette.set(11, 120);
//     palette.set(10, 9999);

//     palette.set(1, 101);
//     palette.set(2, 102);
//     palette.set(3, 103);

//     std::cout << palette.get(10) << '\n';
//     std::cout << palette.get(2) << '\n';
//     std::cout << palette.get(3) << '\n';

//     return 0;
// }

#include <nanobench.h>
#include "world/block/nature/dirt_block.hpp"
#include "world/world.hpp"
#include "core/ecs/system.hpp"
#include <entt/entt.hpp>

struct a
{
    int x[10] = {1};
};
struct b
{
    int x[10] = {1};
};
struct c
{
    int x[10] = {1};
};
struct d
{
    int x[10] = {1};
};

int main()
{
    nith::DirtBlock dirt;
    dirt.setType(nith::DirtType::BASALT);
    // nith::Block &block = dirt;
    // std::cout << block.serialize() << '\n';
    nith::World world;
    auto chunk = world.loadChunk({1, 2, 3});

    std::cout << chunk.getPosition().z << '\n';
}