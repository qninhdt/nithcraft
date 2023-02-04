#pragma once

#include "core/palette.hpp"
#include "core/ecs/system.hpp"
#include "core/ecs/archetype.hpp"
#include "world/block/block.hpp"
#include "graphic/mesh.hpp"

namespace nith
{
    static constexpr u32 CHUNK_SIZE = 32;
    static constexpr u32 CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;
      
    class World;
    struct ChunkData;

    struct ChunkPosition : ivec3
    {
    };

    struct ChunkRenderer
    {
        Mesh mesh;
        bool dirty;

        ChunkRenderer();
    };

    using ChunkArchetype = ecs::Archetype<
        ChunkData,
        ChunkRenderer,
        ChunkPosition>;

    class ChunkSystem : public ecs::System<ChunkSystem, ChunkArchetype>
    {
    };

    class Chunk : public ecs::Entity<Chunk, ChunkSystem>
    {
    public:
        ivec3 getPosition() const;
        ivec3 getWorldPosition() const;

        packed_block getPackedBlock(const uvec3& pos) const;
        IBlock getBlock(const uvec3& pos) const;
        void setBlock(const uvec3& pos, const packed_block& block);

        void render() const;

        World& getWorld();

        void generateMesh();

    private:
        static array<tuple<ivec3, BlockFace>, 6> Directions;
        static array<array<vec3, 2 * 3>, 6> BlockVertices;
        static u32 BlockPosToIndex(const uvec3& pos);

        IBlock getBlockOptimally(const ivec3& pos);
    };


    struct ChunkData
    {
        Palette<packed_block, CHUNK_VOLUME> blocks;
        World* world;
        Chunk neighbors[6];

        ChunkData();
    };
} // namespace nith
