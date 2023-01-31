#include "world/block/block.hpp"
#include "world/block/nature/dirt_block.hpp"

namespace nith
{
    IBlock Block::Unpack(const packed_block &data)
    {
        BlockMaterial material = (BlockMaterial)data.material;
        Block *block = [&]() -> Block *
        {
            switch (material)
            {
            case BlockMaterial::DIRT:
                return new DirtBlock();
            default:
                return nullptr;
            }
        }();

        block->deserializeState(data.data);
        return IBlock{block};
    }

} // namespace nith
