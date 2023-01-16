#pragma once

#include "core/types.hpp"

namespace nith::ecs
{
    using entity_id = u32;

    class entity
    {
    public:
        entity(const u32 &id) : m_id(id) {}

        u32 getID() const
        {
            return m_id;
        }

    private:
        u32 m_id;
    };

} // namespace nith::ecs