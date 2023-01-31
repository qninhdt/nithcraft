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
            ArchetypeBase() : m_componentArray(nullptr) {}

            template <typename U>
            NITH_INLINE U *&getComponentArray()
            {
                if constexpr (std::is_same_v<T, U>)
                    return m_componentArray;
                else
                {
                    static_assert(sizeof...(R) != 0, "Component not found");
                    return m_rest.template getComponentArray<U>();
                }
            }

            template <typename U>
            NITH_INLINE U *&getConstComponentArray() const
            {
                if constexpr (std::is_same_v<T, U>)
                    return m_componentArray;
                else
                {
                    static_assert(sizeof...(R) != 0, "Component not found");
                    return m_rest.template getConstComponentArray<U>();
                }
            }

            NITH_INLINE void callConstructor(const u32 &index)
            {
                new (&m_componentArray[index]) T();
                if constexpr (sizeof...(R) > 0)
                    m_rest.callConstructor(index);
            }

            NITH_INLINE void callDeconstructor(const u32 &index)
            {
                if constexpr (sizeof...(R) > 0)
                    m_rest.callDeconstructor(index);
                m_componentArray[index].~T();
            }

            NITH_INLINE void resizeComponentArrays(const u32 &oldSize, const u32 &newSize)
            {
                auto oldArray = m_componentArray;
                m_componentArray = (T *)new u8[newSize * sizeof(T)];

                if (oldArray)
                {
                    for (u32 i = 0; i < oldSize; ++i)
                        m_componentArray[i] = oldArray[i];

                    delete[] ((char *)oldArray);
                }

                if constexpr (sizeof...(R) > 0)
                    m_rest.resizeComponentArrays(oldSize, newSize);
            }

            NITH_INLINE void freeComponentArrays()
            {
                delete[] ((char *)m_componentArray);
                if constexpr (sizeof...(R) > 0)
                    m_rest.freeComponentArrays();
            }

            NITH_INLINE void copyComponents(const u32 &from, const u32 &to)
            {
                m_componentArray[to] = std::move(m_componentArray[from]);
                if constexpr (sizeof...(R) > 0)
                    m_rest.copyComponents(from, to);
            }

        private:
            ArchetypeBase<R...> m_rest;
            mutable T *m_componentArray;
        };
    }

    template <typename... T>
    class Archetype
    {
    public:
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

            m_base.callConstructor(index);
        }

        void removeEntity(const entity_id &id)
        {
            const auto it = m_entityToIndex.find(id);
            const auto index = it->second - 1;
            const auto lastIndex = size() - 1;
            const auto lastEntity = m_indexToEntity[lastIndex];

            m_base.callDeconstructor(index);

            m_base.copyComponents(lastIndex, index);

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
        NITH_INLINE U &getConstComponent(const entity_id &id) const
        {
            // assert
            return m_base.template getConstComponentArray<U>()[m_entityToIndex[id] - 1];
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

        template <typename U>
        NITH_INLINE U &getConstComponentAt(const u32 &index) const
        {
            // assert
            return m_base.template getConstComponentArray<U>()[index - 1];
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

        ~Archetype()
        {
            for (auto &[entity, index] : m_entityToIndex)
            {
                m_base.callDeconstructor(index);
            }

            m_base.freeComponentArrays();
        }

    protected:
        void reserve(const u32 &newSize)
        {
            if (newSize <= capacity())
                return;

            // resize index -> entity lookup table
            m_indexToEntity.resize(newSize);

            m_base.resizeComponentArrays(capacity(), newSize);
        }

        internal::ArchetypeBase<T...> m_base;

        mutable vector<entity_id> m_indexToEntity;
        mutable umap<entity_id, u32> m_entityToIndex;
    };

} // namespace nith::ecs