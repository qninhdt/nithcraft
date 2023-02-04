#pragma once

#include "world/block/block.hpp"
#include "world/block/block_material.hpp"

namespace nith
{
    class AirBlock : public StatefulBlock<>
    {
    public:
        AirBlock() : StatefulBlock(BlockMaterial::AIR) {}

        bool canSeeThrough(const BlockFace &) const { return true; }
    };
} // namespace nith
