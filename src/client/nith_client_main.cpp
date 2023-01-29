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
    // nith::DirtBlock dirt;
    // dirt.setType(nith::DirtType::BASALT);
    // // nith::Block &block = dirt;
    // // std::cout << block.serialize() << '\n';
    // nith::World world;
    // world.m_chunkSystem.create<nith::ChunkArchetype>();

    entt::registry r;

    entt::entity entt_shit;

    // ankerl::nanobench::Bench().minEpochIterations(1 << 10).run(
    //     "entt create",
    //     [&]()
    //     {
    //         auto e = r.create();
    //         r.emplace<a>(e);
    //         r.emplace<b>(e);
    //         r.emplace<c>(e);
    //         r.emplace<d>(e);
    //         ankerl::nanobench::doNotOptimizeAway(e);

    //         entt_shit = e;
    //     });

    using at = nith::ecs::Archetype<a, b, c, d>;
    nith::ecs::System<at> s;

    nith::ecs::entity_id qninh_shit;

    // ankerl::nanobench::Bench().minEpochIterations(1 << 18).run(
    //     "qninh create",
    //     [&]()
    //     {
    //         auto e = s.create<at>();
    //         // s.
    //         ankerl::nanobench::doNotOptimizeAway(e);
    //         qninh_shit = e;
    //     });

    // ankerl::nanobench::Bench().minEpochIterations(1 << 20).run(
    //     "entt get",
    //     [&]()
    //     {
    //         auto e = r.get<a, b, c, d>(entt_shit);
    //         ankerl::nanobench::doNotOptimizeAway(e);
    //     });

    // ankerl::nanobench::Bench().minEpochIterations(1 << 20).run(
    //     "qninh get",
    //     [&]()
    //     {
    //         auto &aa = s.getComponent<at, a>(qninh_shit);
    //         auto &bb = s.getComponent<at, b>(qninh_shit);
    //         auto &cc = s.getComponent<at, c>(qninh_shit);
    //         auto &dd = s.getComponent<at, d>(qninh_shit);
    //         aa.x[0] = 240404;
    //         ankerl::nanobench::doNotOptimizeAway(aa);
    //         ankerl::nanobench::doNotOptimizeAway(bb);
    //         ankerl::nanobench::doNotOptimizeAway(cc);
    //         ankerl::nanobench::doNotOptimizeAway(dd);
    //     });

    std::cout << s.getComponent<at, a>(qninh_shit).x[0];
}