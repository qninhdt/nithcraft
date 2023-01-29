#pragma once

#include "core/ecs/id_stack.hpp"

namespace nith::ecs
{
    namespace internal
    {
        template <typename... T>
        struct SystemBase
        {
        };

        template <typename T, typename... R>
        struct SystemBase<T, R...>
        {
            template <typename U>
            NITH_INLINE auto &getArchetype()
            {
                if constexpr (std::is_same_v<T, U>)
                    return archetype;
                else
                {
                    static_assert(sizeof...(R) != 0, "Archetype not found");
                    return rest.template getArchetypeArray<U>();
                }
            }

            T archetype;
            SystemBase<R...> rest;
        };
    } // namespace internal

    template <typename... T>
    class System
    {
    public:
        template <typename A, typename C>
        NITH_INLINE auto &getComponent(const entity_id &entity)
        {
            return m_base.template getArchetype<A>().template getComponent<C>(entity);
        }

        template <typename... A>
        entity_id create()
        {
            // E entity;
            // entity_id &id = ((Entity &)entity).m_id;
            auto id = m_idStack.pop();
            (m_base.template getArchetype<A>().addEntity(id), ...);
            // return entity;
            return id;
        }

        template <typename E, typename... A>
        void remove(const E &entity)
        {
            entity_id &id = ((Entity &)entity).m_id;
            m_idStack.push(id);
            (m_base.template getArchetype<A>().removeEntity(id), ...);
        }

        // private:
        internal::SystemBase<T...> m_base;
        IdStack m_idStack;
    };
} // namespace nith
