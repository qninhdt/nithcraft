#include "world/world.hpp"

namespace nith
{

    Chunk World::loadChunk(const ChunkPosition &position)
    {
        Chunk chunk = m_chunkSystem.create<Chunk, ChunkArchetype>();
        auto &chunkPosition = m_chunkSystem.getComponent<ChunkArchetype, ChunkPosition>(chunk);
        chunkPosition = position;

        return chunk;
    }
} // namespace nith
