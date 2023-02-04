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
            auto &getArchetype()
            {
                if constexpr (std::is_same_v<T, U>)
                    return archetype;
                else
                {
                    static_assert(sizeof...(R) != 0, "Archetype not found");
                    return rest.template getArchetypeArray<U>();
                }
            }

            template <typename U>
            auto &getConstArchetype() const
            {
                if constexpr (std::is_same_v<T, U>)
                    return archetype;
                else
                {
                    static_assert(sizeof...(R) != 0, "Archetype not found");
                    return rest.template getConstArchetypeArray<U>();
                }
            }

            T archetype;
            SystemBase<R...> rest;
        };
    } // namespace internal

    template <typename S, typename... T>
    class System
    {
    public:

        template <typename A, typename C, typename E>
        auto &getComponent(const E &entity)
        {
            return m_base.template getArchetype<A>().template getComponent<C>(entity.getId());
        }

        template <typename A, typename C, typename E>
        auto &getConstComponent(const E &entity) const
        {
            return m_base.template getConstArchetype<A>().template getConstComponent<C>(entity.getId());
        }

        template <typename E, typename... A>
        E create()
        {
            E entity;
            entity_id &id = ((Entity<E, S> &)entity).m_id;
            ((Entity<E, S> &)entity).m_system = (S *)this;
            id = m_idStack.pop();

            (m_base.template getArchetype<A>().addEntity(id), ...);
            return entity;
        }

        template <typename E, typename... A>
        void remove(const E &entity)
        {
            entity_id &id = ((Entity<E, S> &)entity).m_id;
            (m_base.template getArchetype<A>().removeEntity(id), ...);
            m_idStack.push(id);
        }

        template <typename A, typename... C>
        void each(auto callback)
        {
            m_base.template getArchetype<A>().each<C...>(callback);
        }

    private:
        internal::SystemBase<T...> m_base;
        IdStack m_idStack;
    };
} // namespace nith
