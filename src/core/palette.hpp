#pragma once

#include "core/bit_storage.hpp"

namespace nith
{
    template <typename T>
    class Palette
    {
    public:
        Palette() : m_storage(nullptr)
        {
        }

        T get(const u32 &index) const
        {
            return m_idToValue[m_storage->get(index)];
        }

        void set(const u32 &index, const T &value)
        {
            auto &it = m_valueToId.find(value);

            if (it)
            {
                m_storage->set(index, it->second);
            }
            else
            {
                        }
        }

    private:
        umap m_valueToId;
        vector m_idToValue;
        BitStorage *m_storage;
    };
} // namespace nith
