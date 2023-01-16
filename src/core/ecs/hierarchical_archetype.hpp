#pragma once

#include "core/ecs/archetype.hpp"
#include "core/ecs/id_stack.hpp"

namespace nith::ecs
{
    struct HierarchyComponent
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

    namespace internal
    {
        struct NodeComponent
        {
            u32 nextIndex;
            u32 prevIndex;
        };
    }

    template <typename... T>
    class HierarchicalArchetype : public Archetype<HierarchyComponent, internal::NodeComponent, T...>
    {
    public:
        HierarchicalArchetype() : m_firstIndex(0)
        {
        }

        void addEntity(const entity_id &id)
        {
            if (m_freeIndices.empty())
            {
                u32 capacity = this->capacity();
                u32 newCapacity = capacity ? capacity * 2 : 16;
                for (u32 i = capacity + 1; i <= newCapacity; ++i)
                    m_freeIndices.push(i);

                this->reserve(newCapacity);
            }

            u32 index = m_freeIndices.top();
            m_freeIndices.pop();

            // index -> entity
            this->m_indexToEntity[index - 1] = id;

            // entity -> index
            this->m_entityToIndex[id] = index;

            this->m_base.callAttachCallback(index);

            auto &node = this->template getComponentAt<internal::NodeComponent>(index);
            if (index == 1)
            {
                if (m_firstIndex)
                {
                    auto &firstNode = this->template getComponentAt<internal::NodeComponent>(m_firstIndex);
                    node.nextIndex = m_firstIndex;
                    node.prevIndex = 0;
                    firstNode.prevIndex = index;
                }
                m_firstIndex = index;
            }
            else
            {
                auto &prevNode = this->template getComponentAt<internal::NodeComponent>(index - 1);

                if (prevNode.nextIndex)
                {
                    auto &nextNode = this->template getComponentAt<internal::NodeComponent>(prevNode.nextIndex);
                    nextNode.prevIndex = index;
                }

                node.nextIndex = prevNode.nextIndex;
                node.prevIndex = index - 1;
                prevNode.nextIndex = index;
            }
        }

        void removeEntity(const entity_id &id)
        {
            i32 index = this->m_entityToIndex[id];
            auto &hierarchy = this->template getComponentAt<HierarchyComponent>(index);

            if (hierarchy.parent)
            {
                auto &parent = this->template getComponentAt<HierarchyComponent>(hierarchy.parent);

                if (parent.nextSibling)
                {
                    auto &nextSibling = this->template getComponentAt<HierarchyComponent>(hierarchy.nextSibling);
                    nextSibling.prevSibling = hierarchy.prevSibling;
                }

                if (parent.prevSibling)
                {
                    auto &prevSibling = this->template getComponentAt<HierarchyComponent>(hierarchy.prevSibling);
                    prevSibling.nextSibling = hierarchy.nextSibling;
                }

                if (parent.firstChild == (u32)index)
                {
                    parent.firstChild = hierarchy.nextSibling;
                }
            }

            removeEntityChildren(index);
        }
        void addChildEntity(const entity_id &parentId, const entity_id &id)
        {
            assert(this->m_entityToIndex.find(parentId) != this->m_entityToIndex.end() && "đéo có");
            auto &array = this->m_base.template getComponentArray<HierarchyComponent>();

            this->addEntity(id);

            u32 parentIndex = this->m_entityToIndex[parentId];
            u32 childIndex = this->m_entityToIndex[id];

            auto &parent = array[parentIndex - 1];
            auto &child = array[childIndex - 1];

            child.parent = parentIndex;

            if (parent.firstChild)
            {
                auto &firstChild = array[parent.firstChild - 1];
                firstChild.prevSibling = childIndex;
                child.nextSibling = parent.firstChild;
            }

            parent.firstChild = childIndex;
        }

        template <typename... C>
        void each(auto callback)
        {
            u32 index = m_firstIndex;
            while (index)
            {
                auto &node = this->template getComponentAt<internal::NodeComponent>(index);
                callback(this->template getComponentAt<C>(index)...);
                index = node.nextIndex;
            }
        }

    private:
        void removeEntityChildren(const u32 &index)
        {
            auto &hierarchy = this->template getComponentAt<HierarchyComponent>(index);
            auto &node = this->template getComponentAt<internal::NodeComponent>(index);

            if (hierarchy.firstChild)
            {
                u32 childIndex = hierarchy.firstChild;

                while (childIndex)
                {
                    auto &child = this->template getComponentAt<HierarchyComponent>(childIndex);
                    u32 nextSibling = child.nextSibling;
                    removeEntityChildren(childIndex);
                    childIndex = nextSibling;
                }
            }

            if (m_firstIndex == index)
            {
                if (node.nextIndex)
                {
                    auto &nextNode = this->template getComponentAt<internal::NodeComponent>(node.nextIndex);
                    nextNode.prevIndex = 0;
                }
                m_firstIndex = node.nextIndex;
            }
            else
            {
                if (node.prevIndex)
                {
                    auto &prevNode = this->template getComponentAt<internal::NodeComponent>(node.prevIndex);
                    prevNode.nextIndex = node.nextIndex;
                }
                if (node.nextIndex)
                {
                    auto &nextNode = this->template getComponentAt<internal::NodeComponent>(node.nextIndex);
                    nextNode.prevIndex = node.prevIndex;
                }
            }

            this->m_entityToIndex.erase(this->m_indexToEntity[index - 1]);
            this->m_indexToEntity[index - 1] = 0;
            m_freeIndices.push(index);
        }

        u32 m_firstIndex;
        std::priority_queue<u32, std::vector<u32>, std::greater<u32>> m_freeIndices;
    };

} // namespace nith::ecs
