#pragma once

#include "world/block/block_material.hpp"
#include "world/block/block.hpp"

namespace nith
{
    class GrassBlock : public StatefulBlock<>
    {
    public:
        GrassBlock() : StatefulBlock(BlockMaterial::GRASS)
        {
        }

        u32 getTextureId(const BlockFace& face) const
        {
            if (face == BlockFace::UP)
                return 0;

            if (face == BlockFace::DOWN)
                return 1;

            return 2;
        }
    };
} // namespace nith
