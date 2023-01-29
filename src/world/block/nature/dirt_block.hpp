#pragma once

#include "world/block/block_material.hpp"
#include "world/block/block.hpp"

namespace nith
{
    enum class DirtType
    {
        SOIL,
        BASALT
    };

    class DirtBlock
        : public StatefulBlock<
              block::state<DirtType, "type", 1>>
    {
    public:
        DirtBlock() : StatefulBlock(BlockMaterial::DIRT)
        {
        }

        DirtType getType() const
        {
            return this->get<"type">();
        }

        void setType(const DirtType &type)
        {
            return this->set<"type">(type);
        }
    };
} // namespace nith
