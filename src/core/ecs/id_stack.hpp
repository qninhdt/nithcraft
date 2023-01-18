#pragma once

#include "core/ecs/entity.hpp"

namespace nith::ecs
{
    class IdStack
    {
    public:
        IdStack() : m_capacity(0)
        {
        }

        entity_id pop()
        {
            if (m_freeIdStack.empty())
            {
                u32 newCapacity = m_capacity ? m_capacity * 2 : 16;
                for (u32 i = newCapacity; i >= m_capacity + 1; --i)
                    m_freeIdStack.push(i);
                m_capacity = newCapacity;
            }

            entity_id id = m_freeIdStack.top();
            m_freeIdStack.pop();

            return id;
        }

        bool empty() const
        {
            return m_freeIdStack.empty();
        }

        void push(const entity_id &id)
        {
            m_freeIdStack.push(id);
        }

    private:
        u32 m_capacity;
        stack<entity_id> m_freeIdStack;
    };
} // namespace nith::ecs
