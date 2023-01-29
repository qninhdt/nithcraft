#pragma once

#include "world/chunk/chunk.hpp"

namespace nith
{
    class World
    {
    public:
        Chunk loadChunk(const ChunkPosition &position)
        {
            Chunk chunk = m_chunkSystem.create<Chunk, ChunkArchetype>();
            auto &chunkPosition = m_chunkSystem.getComponent<ChunkArchetype, ChunkPosition>(chunk);
            chunkPosition = position;

            return chunk;
        }

        ChunkSystem m_chunkSystem;
    };
} // namespace nith
