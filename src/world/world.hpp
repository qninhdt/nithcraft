#pragma once

#include "world/chunk/chunk.hpp"

namespace nith
{
    class World
    {
    public:
        Chunk loadChunk(const ChunkPosition &position);

        IBlock getBlock(const ivec3 &pos)
        {
            ivec3 chunkPos = BlockPosToChunkPos(pos);
            uvec3 localPos = WorldPosToLocalPos(pos);

            auto it = m_chunkMap.find(pos);

            Chunk &chunk = it->second;

            return Block::Unpack(chunk.getBlock(localPos));
        }

        void setBlock(const ivec3 &pos, IBlock block)
        {
            setBlock(pos, *block);
        }

        void setBlock(const ivec3 &pos, const Block &block)
        {
        }

        void setPackedBlock();

    private:
        static ivec3 BlockPosToChunkPos(const ivec3 &pos)
        {
            return {pos.x / CHUNK_SIZE, pos.y / CHUNK_SIZE, pos.z / CHUNK_SIZE};
        }

        static uvec3 WorldPosToLocalPos(const ivec3 &pos)
        {
            return {pos.x & (CHUNK_SIZE - 1), pos.y & (CHUNK_SIZE - 1), pos.z & (CHUNK_SIZE - 1)};
        }

        ChunkSystem m_chunkSystem;
        umap<ivec3, Chunk> m_chunkMap;
    };
} // namespace nith
