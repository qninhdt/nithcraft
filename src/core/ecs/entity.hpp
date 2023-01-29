#pragma once

#include "core/types.hpp"

namespace nith::ecs
{
    using entity_id = u32;

    class Entity
    {
    public:
        entity_id getId() const
        {
            return m_id;
        }

    private:
        template <typename... T>
        friend class System;

        entity_id m_id;
    };
} // namespace nith::ecs