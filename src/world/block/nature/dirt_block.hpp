#pragma once

#include "world/block/block_material.hpp"
#include "world/block/block.hpp"

namespace nith
{
    class DirtBlock : public StatefulBlock<>
    {
    public:
        DirtBlock() : StatefulBlock(BlockMaterial::DIRT)
        {
        }

        u32 getTextureId(const BlockFace& face) const
        {
            return 3;
        }
    };
} // namespace nith
