#pragma once

#include "util/math.hpp"
#include "world/block/block_material.hpp"
#include "core/const_string.hpp"

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

        u64 serializeState() const
        {
            u64 output = 0;

            NITH_UNUSED bool dont_care_me;
            (dont_care_me = ... = (serializeOne<S>(output), 0)); // trick

            return output;
        }

        void deserializeState(const u64 &input)
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

    union packed_block
    {
        struct
        {
            u32 material;
            u32 state;
        };

        u64 data;

        bool operator==(const packed_block &other) const
        {
            return data == other.data;
        }
    };

    struct BlockFace
    {
        enum Enum
        {
            NORTH,
            EAST,
            UP,
            SOUTH,
            WEST,
            DOWN
        };

        static Enum FromDirection(const ivec3 &dir)
        {
            if (dir.z == 1)
                return NORTH;
            if (dir.z == -1)
                return SOUTH;
            if (dir.y == 1)
                return UP;
            if (dir.y == -1)
                return DOWN;
            if (dir.x == 1)
                return EAST;
            if (dir.x == -1)
                return WEST;

            return NORTH;
        }
    };

    class Chunk;
    class Block;

    using IBlock = std::unique_ptr<Block>;

    class Block
    {
    public:
        Block(const BlockMaterial &material) : m_material(material), m_chunk(nullptr)
        {
        }

        BlockMaterial getMaterial() const
        {
            return m_material;
        }

        virtual u32 serializeState() const { return 0; };

        virtual void deserializeState(const u32 &state){};

        virtual bool canSeeThrough(const BlockFace::Enum &face) { return false; };
        static IBlock Unpack(const packed_block &data);

    private:
        BlockMaterial m_material;
        Chunk *m_chunk;
    };

    template <typename... S>
    class StatefulBlock : public Block, public StateHolder<S...>
    {
    public:
        StatefulBlock(const BlockMaterial &material) : Block(material)
        {
        }

        u32 serializeState() const
        {
            return StateHolder<S...>::serializeState();
        }

        void deserializeState(const u32 &state)
        {
            StateHolder<S...>::deserializeState(state);
        }
    };
} // namespace nith

namespace std
{
    template <>
    struct hash<nith::packed_block>
    {
        size_t operator()(const nith::packed_block &block) const
        {
            return hash<nith::u64>()(block.data);
        }
    };
}
