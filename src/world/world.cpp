#include "world/world.hpp"
#include "world/block/nature/dirt_block.hpp"
#include "world/block/nature/grass_block.hpp"
#include "application.hpp"
#include "game.hpp"

namespace nith
{
    World::World()
    {
        m_chunkShader = Application::Get().getShaderManager().getShader<BasicShader>("basic");
    }

    Chunk World::getChunk(const ivec3 &pos)
    {
        auto it = m_chunkMap.find(pos);
        if (it == m_chunkMap.end())
            return {};
        return it->second;
    }

    Chunk World::loadChunk(const ivec3 &position)
    {
        Chunk chunk = m_chunkSystem.create<Chunk, ChunkArchetype>();
        auto &chunkPosition = m_chunkSystem.getComponent<ChunkArchetype, ChunkPosition>(chunk);
        auto &chunkData = m_chunkSystem.getComponent<ChunkArchetype, ChunkData>(chunk);
        chunkData.world = this;
        (ivec3 &)chunkPosition = position;
        m_chunkMap[position] = chunk;

        for (u32 x = 0; x < CHUNK_SIZE; ++x)
            for (u32 z = 0; z < CHUNK_SIZE; ++z)
            {
                u32 height = sqrt((x * x + z * z)/2);

                if (height >= 1)
                {
                    for (u32 y = 0; y < height - 1; ++y)
                        chunk.setBlock({ x, y, z }, Block::Pack(DirtBlock{}));
                    chunk.setBlock({ x, height - 1, z }, Block::Pack(GrassBlock{}));
                }
            }

        chunk.generateMesh();
        return chunk;
    }

    void World::unloadChunk(const ivec3 &position)
    {
        auto it = m_chunkMap.find(position);

        assert(it != m_chunkMap.end());

        Chunk &chunk = it->second;

        m_chunkSystem.remove<Chunk, ChunkArchetype>(chunk);

        m_chunkMap.erase(it);
    }

    IBlock World::getBlock(const ivec3 &pos)
    {
        ivec3 chunkPos = BlockPosToChunkPos(pos);
        uvec3 localPos = WorldPosToLocalPos(pos);

        auto it = m_chunkMap.find(chunkPos);

        if (it == m_chunkMap.end())
            return nullptr;

        Chunk &chunk = it->second;

        return chunk.getBlock(localPos);
    }

    void World::setBlock(const ivec3 &pos, IBlock block)
    {
        setBlock(pos, *block);
    }

    void World::setBlock(const ivec3 &pos, const Block &block)
    {
        setPackedBlock(pos, Block::Pack(block));
    }

    void World::setPackedBlock(const ivec3 &pos, const packed_block &block)
    {
        ivec3 chunkPos = BlockPosToChunkPos(pos);
        uvec3 localPos = WorldPosToLocalPos(pos);

        auto it = m_chunkMap.find(chunkPos);

        Chunk &chunk = it->second;

        chunk.setBlock(localPos, block);
    }

    void World::tick(const f32& deltaTime)
    {
    }

    void World::render(const f32& deltaTime)
    {
        static f32 x = 0;
        x += 0.001f;
        auto& app = Application::Get();
        auto& game = Game::Get();

        app.getBlockTextureManager().use();
        m_chunkShader.use();

        m_chunkSystem.each<ChunkArchetype, ChunkPosition, ChunkRenderer>(
            [&](ChunkPosition& position, ChunkRenderer& renderer)
            {
                vec3 worldPos = position.toWorldPosition();
                std::cout << worldPos.x << ' ' << worldPos.y << ' ' << worldPos.z << '\n';
                mat4 model = glm::translate(glm::mat4(1), worldPos);
                //model = glm::rotate(model, x, vec3(1.0f, 0.0f, 0.0f));
                m_chunkShader.setUniform<"mvp">(game.getCamera().getProjectionView() * model);
                renderer.mesh.drawTriangles();
            }
        );
    }

    umap<ivec3, Chunk> World::getChunkMap() const
    {
        return m_chunkMap;
    }

    ivec3 World::BlockPosToChunkPos(const ivec3 &pos)
    {
        return {pos.x >> 5, pos.y >> 5, pos.z >> 5};
    }

    uvec3 World::WorldPosToLocalPos(const ivec3 &pos)
    {
        return {pos.x & (CHUNK_SIZE - 1), pos.y & (CHUNK_SIZE - 1), pos.z & (CHUNK_SIZE - 1)};
    }
} // namespace nith
