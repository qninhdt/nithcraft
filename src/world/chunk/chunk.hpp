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

    class ChunkSystem : public ecs::System<ChunkSystem, ChunkArchetype>
    {
    };

    class Chunk : public ecs::Entity<Chunk, ChunkSystem>
    {
    public:
        static constexpr u32 CHUNK_WIDTH = 32;
        static constexpr u32 CHUNK_HEIGHT = 32;
        static constexpr u32 CHUNK_DEPTH = 32;
        static constexpr u32 CHUNK_VOLUME = CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH;

        Chunk(ChunkSystem &system) : ecs::Entity<Chunk, ChunkSystem>(system)
        {
        }

        ChunkPosition &getPosition() const
        {
            return this->getConstComponent<ChunkArchetype, ChunkPosition>();
        }
    };
} // namespace nith
