#pragma once

#include "core/ecs/archetype.hpp"

namespace nith
{
    struct Hierarchy
    {
        u32 parent;
        u32 nextSibling;
        u32 prevSibling;
        u32 firstChild;

        void onAttach()
        {
            parent = 0;
            nextSibling = 0;
            prevSibling = 0;
            firstChild = 0;
        }
    };

    template <typename E, typename... T>
    class HierarchicalArchetype : public Archetype<E, Hierarchy, T...>
    {
    public:
        HierarchicalArchetype() = default;

        void removeEntity(const entity_id &id)
        {
            i32 index = this->m_entityToIndex[id];
            i32 lastIndex = this->size();
            vector<i32> removedIndices;

            removeEntityChildren(index, removedIndices);

            std::sort(removedIndices.begin(), removedIndices.end());

            i32 nRemovedIndices = removedIndices.size();
            for (i32 i = lastIndex, j = 0; i >= lastIndex - nRemovedIndices + 1 && removedIndices[j] < i; --i)
            {
                if (this->m_indexToEntity[i - 1])
                {
                    moveItem(i, removedIndices[j++]);
                }
            }
        }

        void addChildEntity(const entity_id &parentId, const entity_id &id)
        {
            auto &array = this->m_base.template getComponentArray<Hierarchy>();

            this->addEntity(id);

            u32 parentIndex = this->m_entityToIndex[parentId];
            u32 childIndex = this->m_entityToIndex[id];

            Hierarchy &parent = array[parentIndex - 1];
            Hierarchy &child = array[childIndex - 1];

            child.parent = parentIndex;

            if (!parent.firstChild)
            {
                parent.firstChild = childIndex;
            }
        }

    private:
        void copyItem(const u32 &from, const u32 &to)
        {
            this->m_base.copyComponents(from, to);

            this->m_entityToIndex[this->m_indexToEntity[from - 1]] = to;
            this->m_indexToEntity[to - 1] = this->m_indexToEntity[from - 1];
            this->m_indexToEntity[from - 1] = 0;
        }

        u32 moveItem(const u32 &from, const u32 &to)
        {
            Hierarchy &child = this->template getComponentAt<Hierarchy>(from);

            if (child.parent && child.parent > to)
            {
                Hierarchy &parent = this->template getComponentAt<Hierarchy>(child.parent);
                if (parent.firstChild == from)
                {
                    parent.firstChild = child.parent;
                }
                u32 real = child.parent;
                child.parent = moveItem(child.parent, to);
                copyItem(from, real);

                return real;
            }
            else
            {
                copyItem(from, to);

                return to;
            }
        }

        void removeEntityChildren(const u32 &index, vector<i32> &removedIndices)
        {
            Hierarchy &hierarchy = this->template getComponentAt<Hierarchy>(index);

            this->m_entityToIndex.erase(this->m_indexToEntity[index - 1]);
            this->m_indexToEntity[index - 1] = 0;
            removedIndices.push_back(index);

            if (hierarchy.firstChild)
            {
                u32 childIndex = hierarchy.firstChild;

                while (childIndex)
                {
                    Hierarchy &child = this->template getComponentAt<Hierarchy>(childIndex);

                    removeEntityChildren(childIndex, removedIndices);

                    childIndex = child.nextSibling;
                }
            }
        }
    };

} // namespace nith
