#pragma once

#include "world/chunk/chunk.hpp"

namespace nith
{
    class World
    {
    public:
        Chunk getChunk(const ivec3 &position);
        Chunk loadChunk(const ivec3 &position);
        void unloadChunk(const ivec3 &position);

        IBlock getBlock(const ivec3 &pos);
        void setBlock(const ivec3 &pos, IBlock block);
        void setBlock(const ivec3 &pos, const Block &block);
        void setPackedBlock(const ivec3 &pos, const packed_block &block);

        void tick(const f32& deltaTime);
        void render(const f32& deltaTime);

        umap<ivec3, Chunk> getChunkMap() const;

    private:
        static ivec3 BlockPosToChunkPos(const ivec3 &pos);
        static uvec3 WorldPosToLocalPos(const ivec3 &pos);

        ChunkSystem m_chunkSystem;
        umap<ivec3, Chunk> m_chunkMap;
    };
} // namespace nith
