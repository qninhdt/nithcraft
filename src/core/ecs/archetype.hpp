#pragma once

#include <type_traits>
#include <iostream>

#include "core/ecs/entity.hpp"

namespace nith
{
    constexpr u32 ACHETYPE_DEFAULT_SIZE = 2;

    template <typename... T>
    class AchetypeBase
    {
    };

    template <typename T, typename... R>
    class AchetypeBase<T, R...>
    {
    public:
        template <typename U>
        NITH_INLINE vector<U> &getArray()
        {
            if constexpr (std::is_same_v<T, U>)
                return m_componentArray;
            else
            {
                static_assert(sizeof...(R) != 0, "Component not found");
                return m_rest.template getArray<U>();
            }
        }

        NITH_INLINE void resizeArrays(const u32 &newSize)
        {
            // std::cout << typeid(T).name() << ": " << newSize << '\n';
            m_componentArray.resize(newSize);
            if constexpr (sizeof...(R) > 0)
                m_rest.resizeArrays(newSize);
        }

        NITH_INLINE void swapItemOfArrays(const u32 &a, const u32 &b)
        {
            std::swap(m_componentArray[a], m_componentArray[b]);
            if constexpr (sizeof...(R) > 0)
                m_rest.swapItemOfArrays(a, b);
        }

    private:
        AchetypeBase<R...> m_rest;
        vector<T> m_componentArray;
    };

    template <typename E, typename... T>
    class Achetype : public AchetypeBase<T...>
    {
    public:
        Achetype()
        {
            reserve(ACHETYPE_DEFAULT_SIZE);
        }

        void addEntity(const E &e)
        {
            if (m_freeIndices.empty())
                reserve(capacity() << 1);

            // get a free index
            u32 index = m_freeIndices.top();
            m_freeIndices.pop();

            // index -> entity
            m_indexToEntity[index] = e.getID();

            // entity -> index
            m_entityToIndex[e.getID()] = index;
        }

        void removeEntity(const E &id)
        {
            const auto it = m_entityToIndex.find(id);
            const auto index = it->second;
            const auto lastIndex = size() - 1;
            const auto lastEntity = m_indexToEntity[lastIndex];

            this->swapItemOfArrays(index, lastIndex);

            // update index of last entity
            m_entityToIndex[lastEntity] = index;

            // move entity to deleted index
            m_indexToEntity[index] = lastEntity;

            m_freeIndices.push(index);
            m_indexToEntity[index] = 0;
            m_entityToIndex.erase(it);
        }

        template <typename U>
        U &getComponent(const entity &e)
        {
            // assert
            return this->template getArray<U>()[m_entityToIndex[e.getID()]];
        }

        template <typename U>
        void setComponent(const entity &e, const U &component)
        {
            this->template getArray<U>()[m_entityToIndex[e.getID()]] = component;
        }

        template <typename U>
        U &getComponentAt(const u32 &index)
        {
            // assert
            return this->template getArray<U>()[index];
        }

        u32 size() const
        {
            return m_entityToIndex.size();
        }

        u32 capacity() const
        {
            return m_indexToEntity.size();
        }

        void print()
        {
            for (auto i : m_indexToEntity)
            {
                if (i)
                    std::cout << i << ' ';
                else
                    std::cout << "_ ";
            }
            std::cout << '\n';
            for (auto [entity, index] : m_entityToIndex)
                std::cout << entity << " -> " << index << '\n';
            std::cout << '\n';
        }

        // private:
        void reserve(const u32 &newSize)
        {
            const u32 oldSize = capacity();

            if (newSize <= oldSize)
                return;

            // resize index -> entity lookup table
            m_indexToEntity.resize(newSize);

            // insert new free indices
            for (u32 index = newSize; index > oldSize; --index)
            {
                m_freeIndices.push(index - 1);
            }

            this->resizeArrays(newSize);
        }

        stack<u32> m_freeIndices;
        vector<entity_id> m_indexToEntity;
        umap<entity_id, u32> m_entityToIndex;
    };

} // namespace nith