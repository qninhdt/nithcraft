#pragma once

#include "core/types.hpp"

namespace nith::ecs
{
    using entity_id = u32;

    template <typename D, typename S>
    class Entity
    {
    public:
        Entity(S &system) : m_system(system) {}

        entity_id getId() const
        {
            return m_id;
        }

        template <typename A, typename C>
        NITH_INLINE auto &getComponent()
        {
            return m_system.template getComponent<A, C>((D &)*this);
        }

        template <typename A, typename C>
        NITH_INLINE auto &getConstComponent() const
        {
            return m_system.template getConstComponent<A, C>((D &)*this);
        }

    private:
        template <typename SS, typename... T>
        friend class System;

        S &m_system;
        entity_id m_id;
    };
} // namespace nith::ecs