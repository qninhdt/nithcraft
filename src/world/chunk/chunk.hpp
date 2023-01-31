#pragma once

#include "core/palette.hpp"
#include "core/ecs/system.hpp"
#include "core/ecs/archetype.hpp"
#include "world/block/block.hpp"
#include "client/graphic/mesh.hpp"

namespace nith
{
    static constexpr u32 CHUNK_SIZE = 32;
    static constexpr u32 CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

    struct ChunkPosition : uvec3
    {
        using uvec3::vec;
    };

    struct ChunkData
    {
        Palette<packed_block, CHUNK_VOLUME> blocks;

        void onDetach()
        {
            blocks.reset();
        }
    };

    struct ChunkMesh
    {
        Mesh mesh;

        ChunkMesh()
        {
            std::cout << "construct mesh";
        }

        ~ChunkMesh()
        {
            std::cout << "deconstruct mesh";
        }
    };

    using ChunkArchetype = ecs::Archetype<
        ChunkData,
        ChunkPosition>;

    class ChunkSystem : public ecs::System<ChunkSystem, ChunkArchetype>
    {
    };

    class Chunk : public ecs::Entity<Chunk, ChunkSystem>
    {
    public:
        Chunk(ChunkSystem &system) : ecs::Entity<Chunk, ChunkSystem>(system)
        {
        }

        ChunkPosition &getPosition() const
        {
            return this->getConstComponent<ChunkArchetype, ChunkPosition>();
        }

        packed_block getBlock(const uvec3 &pos) const
        {
            auto &data = this->getConstComponent<ChunkArchetype, ChunkData>();
            return data.blocks.get(Chunk::BlockPosToIndex(pos));
        }

        void setBlock(const uvec3 &pos, const packed_block &block)
        {
            auto &data = this->getComponent<ChunkArchetype, ChunkData>();
            data.blocks.set(Chunk::BlockPosToIndex(pos), block);
        }

        void generateMesh();

    private:
        static u32 BlockPosToIndex(const uvec3 &pos)
        {
            return pos.x * CHUNK_SIZE * CHUNK_SIZE + pos.y * CHUNK_SIZE + pos.z;
        }
    };
} // namespace nith
