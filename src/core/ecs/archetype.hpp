#pragma once

#include <type_traits>
#include <iostream>

#include "core/ecs/entity.hpp"

namespace nith::ecs
{
    namespace internal
    {
        constexpr u32 ARCHETYPE_DEFAULT_SIZE = 2;

        template <typename... T>
        class ArchetypeBase
        {
        };

        template <typename T, typename... R>
        class ArchetypeBase<T, R...>
        {
        public:
            template <typename U>
            NITH_INLINE vector<U> &getComponentArray()
            {
                if constexpr (std::is_same_v<T, U>)
                    return m_componentArray;
                else
                {
                    static_assert(sizeof...(R) != 0, "Component not found");
                    return m_rest.template getComponentArray<U>();
                }
            }

            NITH_INLINE void callAttachCallback(const u32 &index)
            {
                if constexpr (requires { T::onAttach; })
                    m_componentArray[index].onAttach();
                if constexpr (sizeof...(R) > 0)
                    m_rest.callAttachCallback(index);
            }

            NITH_INLINE void resizeComponentArrays(const u32 &newSize)
            {
                // std::cout << typeid(T).name() << ": " << newSize << '\n';
                m_componentArray.resize(newSize);
                if constexpr (sizeof...(R) > 0)
                    m_rest.resizeComponentArrays(newSize);
            }

            NITH_INLINE void copyComponents(const u32 &from, const u32 &to)
            {
                m_componentArray[to - 1] = std::move(m_componentArray[from - 1]);
                if constexpr (sizeof...(R) > 0)
                    m_rest.copyComponents(from, to);
            }

        private:
            ArchetypeBase<R...> m_rest;
            vector<T> m_componentArray;
        };
    }

    template <typename... T>
    class Archetype
    {
        friend class iterator;

    public:
        Archetype()
        {
        }

        void addEntity(const entity_id &id)
        {
            if (size() == capacity())
                reserve(capacity() ? capacity() * 2 : 16);

            // get a free index
            u32 index = size();

            // index -> entity
            m_indexToEntity[index] = id;

            // entity -> index
            m_entityToIndex[id] = index + 1;

            m_base.callAttachCallback(index);
        }

        void removeEntity(const entity_id &id)
        {
            const auto it = m_entityToIndex.find(id);
            const auto index = it->second - 1;
            const auto lastIndex = size() - 1;
            const auto lastEntity = m_indexToEntity[lastIndex];

            m_base.copyComponents(lastIndex, index - 1);

            m_indexToEntity[index] = lastEntity;
            m_entityToIndex[lastEntity] = index + 1;

            m_indexToEntity[lastIndex] = 0;
            m_entityToIndex.erase(it);
        }

        template <typename U>
        NITH_INLINE U &getComponent(const entity_id &id)
        {
            // assert
            return m_base.template getComponentArray<U>()[m_entityToIndex[id] - 1];
        }

        template <typename U>
        NITH_INLINE void setComponent(const entity_id &id, const U &component)
        {
            m_base.template getComponentArray<U>()[m_entityToIndex[id] - 1] = component;
        }

        template <typename U>
        NITH_INLINE U &getComponentAt(const u32 &index)
        {
            // assert
            return m_base.template getComponentArray<U>()[index - 1];
        }

        NITH_INLINE u32 size() const
        {
            return m_entityToIndex.size();
        }

        NITH_INLINE u32 capacity() const
        {
            return m_indexToEntity.size();
        }

        template <typename... C>
        void each(std::function<void(C...)> callback)
        {
        }

        void print()
        {
            for (auto i : m_indexToEntity)
            {
                if (i)
                    std::cout << std::setw(4) << i;
                else
                    std::cout << std::setw(4) << "__";
            }
            std::cout << '\n';
            // for (auto [entity, index] : m_entityToIndex)
            //     std::cout << entity << " -> " << index << '\n';
            // std::cout << '\n';
        }

    protected:
        void reserve(const u32 &newSize)
        {
            if (newSize <= capacity())
                return;

            // resize index -> entity lookup table
            m_indexToEntity.resize(newSize);

            m_base.resizeComponentArrays(newSize);
        }

        internal::ArchetypeBase<T...> m_base;

        vector<entity_id> m_indexToEntity;
        umap<entity_id, u32> m_entityToIndex;
    };

} // namespace nith::ecs