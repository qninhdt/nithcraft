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

            template <const_string name>
            auto &getHolder()
            {
                if constexpr (S::name == name)
                    return *this;
                else
                {
                    static_assert(sizeof...(R) > 0 && "Unknown state");
                    return rest.template getHolder<name>();
                }
            }

            template <const_string name>
            auto &getConstHolder() const
            {
                if constexpr (S::name == name)
                    return *this;
                else
                {
                    static_assert(sizeof...(R) > 0 && "Unknown state");
                    return rest.template getConstHolder<name>();
                }
            }

            T value;
            SingleStateHolder<R...> rest;
        };

    }

    namespace block
    {   
        template <typename T, const_string pname, u32 pnum_bits>
        struct state
        {
            using type = T;
            static constexpr const_string name = pname;
            static constexpr u32 num_bits = pnum_bits;
            static constexpr u32 name_length = name.length();

            T value;
        };

        template <const_string name, u32 num_bits>
        using int_state = block::state<u32, name, num_bits>;

        template <const_string name>
        using bool_state = block::state<bool, name, 1>;

        template <typename T>
        concept is_state = requires
        {
            typename T::type;
            { T::num_bits } -> std::same_as<const u32&>;
            { T::name_length } -> std::same_as<const u32&>;
            { T::name } -> std::same_as <const const_string<T::name_length>&>;
        };

        template <typename T>
        concept is_state_holder = requires(T t)
        {   
            { t.getHolder() } -> std::same_as<decltype(t.getHolder())>;
        };

        template <is_state_holder T>
        using holder_of = std::remove_reference_t<decltype(T{}.getHolder()) > ;

        template <typename T>
        concept is_state_or_state_holder = is_state<T> || is_state_holder<T>;

        template <is_state_or_state_holder... S>
        class state_holder : public S...
        {
        public:
            template <const_string name>
            auto& get() const
            {
                return get_impl<name, S...>();
            }
            
            template <const_string name>
            static constexpr bool has() {
                return has_impl<name, S...>();
            }

            template <const_string name>
            using type = decltype(type_impl<name, S...>());

            template <const_string name, typename T>
            void set(const T& value)
            {
                static_assert(std::is_same_v<T, type<name>>);
                * (T*)& get<name>() = value;
            }

            state_holder<S...>& getHolder()
            {
                return *this;
            }

        private:

            template <const_string name, is_state T, typename... R>
            static auto type_impl()
            {
                if constexpr (T::name == name)
                    return typename T::type{};
                else
                {
                    static_assert(sizeof...(R) > 0);
                    return type_impl<name, R...>();
                }
            }

            template <const_string name, is_state_holder T, typename... R>
            static auto type_impl()
            {
                if constexpr (T::template has<name>())
                    return typename T::template type<name>{};
                else
                {
                    static_assert(sizeof...(R) > 0);
                    return type_impl<name, R...>();
                }
            }

            template <const_string name, is_state T, typename... R>
            auto& get_impl() const
            {
                if constexpr (T::name == name)
                    return ((T*)this)->value;
                else 
                {
                    static_assert(sizeof...(R) > 0);
                    return get_impl<name, R...>();
                }
            }

            template <const_string name, is_state_holder T, typename... R>
            auto& get_impl() const
            {
                if constexpr (T::template has<name>())
                    return T::template get<name>();
                else 
                {
                    static_assert(sizeof...(R) > 0);
                    return get_impl<name, R...>();
                }
            }

            template <const_string name, is_state T, typename ...R>
            static constexpr bool has_impl()
            {
                if constexpr (T::name == name)
                    return true;
                else if constexpr (sizeof...(R) > 0)
                    return has_impl<name, R...>();
                else
                    return false;
            }

            template <const_string name, is_state_holder T, typename ...R>
            static constexpr bool has_impl()
            {
                if constexpr (T::template has<name>())
                    return true;
                
                if constexpr (sizeof...(R) > 0)
                    return has_impl<name, R...>();
                else
                    return false;
            }
        };
    }

    template <typename... S>
    class StateHolder
    {
    public:
        template <const_string name>
        auto get() const
        {
            return m_base.template getConstHolder<name>().value;
        }

        template <const_string name>
        void set(auto value)
        {
            m_base.template getHolder<name>().value = value;
        }

        template <const_string name>
        constexpr bool has() const
        {
            return false;
        }

        u64 serializeState() const
        {
            u64 output = 0;

            bool dont_care_me = 0;
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
        void serializeOne(u64 &output) const
        {
            output = (output << T::num_bits) | ((u64)get<T::name>());
        }

        template <typename T>
        void deserializeOne(u64 &input)
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

    enum class BlockFace
    {
        EAST,
        UP,
        SOUTH,
        WEST,
        DOWN,
        NORTH
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

        virtual void deserializeState(const u32 &){};

        virtual bool canSeeThrough(const BlockFace &) const { return false; };

        virtual uvec3 getColor() const { return {0, 0, 0}; }

        static packed_block Pack(const Block &block);

        static IBlock Unpack(const packed_block &data);
        
        static BlockFace GetFaceFromDirection(const ivec3& dir);

        static BlockFace GetOppositeFace(const BlockFace& face);

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
