#include "world/block/block.hpp"
#include "world/block/nature/air_block.hpp"
#include "world/block/nature/dirt_block.hpp"

namespace nith
{
    packed_block Block::Pack(const Block &block)
    {
        packed_block packed;
        packed.material = (u32)block.getMaterial();
        packed.state = block.serializeState();
        return packed;
    }

    IBlock Block::Unpack(const packed_block &data)
    {
        BlockMaterial material = (BlockMaterial)data.material;
        Block *block = [&]() -> Block *
        {
            switch (material)
            {
            case BlockMaterial::AIR:
                return new AirBlock();
            case BlockMaterial::DIRT:
                return new DirtBlock();
            default:
                return nullptr;
            }
        }();

        block->deserializeState(data.state);
        return IBlock{block};
    }

    BlockFace Block::GetFaceFromDirection(const ivec3& dir)
    {
        if (dir.z == 1)
            return BlockFace::NORTH;
        if (dir.z == -1)
            return BlockFace::SOUTH;
        if (dir.y == 1)
            return BlockFace::UP;
        if (dir.y == -1)
            return BlockFace::DOWN;
        if (dir.x == 1)
            return BlockFace::EAST;
        return BlockFace::WEST;
    }

    BlockFace Block::GetOppositeFace(const BlockFace& face)
    {
        return (BlockFace)(((i32)face + 3) % 6);
    }

} // namespace nith
