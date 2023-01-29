#pragma once

#include "core/palette.hpp"
#include "core/ecs/system.hpp"
#include "world/block/block.hpp"

namespace nith
{
    struct ChunkPosition
    {
        u32 x;
        u32 y;
        u32 z;
    };

    using ChunkArchetype = ecs::Archetype<
        ChunkPosition>;

    using ChunkSystem = ecs::System<ChunkArchetype>;

    class Chunk
    {
    public:
        static constexpr u32 CHUNK_WIDTH = 32;
        static constexpr u32 CHUNK_HEIGHT = 32;
        static constexpr u32 CHUNK_DEPTH = 32;
        static constexpr u32 CHUNK_VOLUME = CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH;

    private:
    };
} // namespace nith
