#pragma once

#include "util/math.hpp"
#include "world/block/block_material.hpp"

namespace nith
{
    namespace internal
    {
        template <typename... S>
        struct SingleStateHolder
        {
        };

        template <typename S, typename... R>
        struct SingleStateHolder<S, R...>
        {
            using T = typename S::type;

            template <const_string NAME>
            NITH_INLINE auto &getHolder()
            {
                if constexpr (S::name == NAME)
                    return *this;
                else
                {
                    static_assert(sizeof...(R) > 0 && "Unknown state");
                    return rest.template getHolder<NAME>();
                }
            }

            template <const_string NAME>
            NITH_INLINE auto &getConstHolder() const
            {
                if constexpr (S::name == NAME)
                    return *this;
                else
                {
                    static_assert(sizeof...(R) > 0 && "Unknown state");
                    return rest.template getConstHolder<NAME>();
                }
            }

            auto &self()
            {
                return *this;
            }

            T value;
            SingleStateHolder<R...> rest;
        };

    }

    namespace block
    {
        template <typename T, const_string NAME, u32 NUM_BITS>
        struct state
        {
            using type = T;
            static constexpr const_string name = NAME;
            static constexpr u32 num_bits = NUM_BITS;
        };

        template <const_string NAME, u32 NUM_BITS>
        using int_state = block::state<u32, NAME, NUM_BITS>;

        template <const_string NAME>
        using bool_state = block::state<bool, NAME, 1>;
    }

    template <typename... S>
    class StateHolder
    {
    public:
        template <const_string NAME>
        auto get() const
        {
            return m_base.template getConstHolder<NAME>().value;
        }

        template <const_string NAME>
        void set(auto value)
        {
            m_base.template getHolder<NAME>().value = value;
        }

        u64 serialize() const
        {
            u64 output = 0;

            NITH_UNUSED bool dont_care_me;
            (dont_care_me = ... = (serializeOne<S>(output), 0)); // trick

            return output;
        }

        void deserialize(const u64 &input)
        {
            u64 input0 = input;
            (deserializeOne<S>(input0), ...);
        }

    private:
        template <typename T>
        NITH_INLINE void serializeOne(u64 &output) const
        {
            output = (output << T::num_bits) | ((u64)get<T::name>());
        }

        template <typename T>
        NITH_INLINE void deserializeOne(u64 &input)
        {
            constexpr u64 mask = ~u64(0) >> (64 - T::num_bits);
            set<T::name>((typename T::type)(input & mask));
            input >>= T::num_bits;
        }

        internal::SingleStateHolder<S...> m_base;
    };

    // 1 byte for material
    // 7 bytes data
    using packed_block = u64;

    class Block
    {
    public:
        Block(const BlockMaterial &material) : m_material(material)
        {
        }

        BlockMaterial getMaterial() const
        {
            return m_material;
        }

        virtual packed_block serialize() const
        {
            return (packed_block)m_material;
        }

        virtual void deserialize(const packed_block &data)
        {
            m_material = (BlockMaterial)data;
        }

    private:
        BlockMaterial m_material;
    };

    template <typename... S>
    class StatefulBlock : public Block, public StateHolder<S...>
    {
    public:
        StatefulBlock(const BlockMaterial &material) : Block(material)
        {
        }

        packed_block serialize() const override
        {
            return (StateHolder<S...>::serialize() << 8) | Block::serialize();
        }

        void deserialize(const packed_block &data) override
        {
            Block::deserialize(data);
            StateHolder<S...>::deserialize(data >> 8);
        }
    };

} // namespace nith
