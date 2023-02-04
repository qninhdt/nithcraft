#pragma once

#include "core/types.hpp"

namespace nith::ecs
{
    using entity_id = u32;

    template <typename D, typename S>
    class Entity
    {
    public:
        Entity() : m_id(0), m_system(0) {}

        entity_id getId() const
        {
            return m_id;
        }

        S &getSystem()
        {
            return m_system;
        }

        template <typename A, typename C>
        auto &getComponent()
        {
            return m_system->template getComponent<A, C>((D &)*this);
        }

        template <typename A, typename C>
        auto &getConstComponent() const
        {
            return m_system->template getConstComponent<A, C>((D &)*this);
        }

        bool operator==(const Entity &entity)
        {
            return entity.m_id == m_id;
        }

    private:
        template <typename SS, typename... T>
        friend class System;

        S *m_system;
        entity_id m_id;
    };
} // namespace nith::ecs