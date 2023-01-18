#pragma once

namespace nith
{
    class BitStorage
    {
    public:
        virtual void clear() = 0;
        virtual u32 get(const u32 &index) const = 0;
        virtual void set(const u32 &index, const u32 &value) = 0;
        // virtual u32 getMaxValue() const = 0;
    };

    template <u32 BIT_SIZE, u32 SIZE>
    class SimpleBitStorage : public BitStorage
    {
    public:
        void clear() override
        {
            std::memset(m_data, 0, sizeof(m_data));
        }

        u32 get(const u32 &index) const override
        {
            assert(index < SIZE);

            u32 intIndex = index >> FIVE_MINUS_BIT_SIZE;
            u32 bitOffset = (index & VALUES_PER_INT_MINUS_ONE) << BIT_SIZE;

            return (m_data[intIndex] >> bitOffset) & BIT_MASK;
        }

        void set(const u32 &index, const u32 &value) override
        {
            assert(index < SIZE);
            assert(value <= MAX_VALUE);

            u32 intIndex = index >> FIVE_MINUS_BIT_SIZE;
            u32 bitOffset = (index & VALUES_PER_INT_MINUS_ONE) << BIT_SIZE;
            u32 clearMask = ~(BIT_MASK << bitOffset);

            m_data[intIndex] = (m_data[intIndex] & clearMask) | (value << bitOffset);
        }

    private:
        constexpr static u32 BITS_PER_VALUE = 1 << BIT_SIZE;
        constexpr static u32 FIVE_MINUS_BIT_SIZE = 5 - BIT_SIZE;
        constexpr static u32 VALUES_PER_INT = 32 >> BIT_SIZE;
        constexpr static u32 VALUES_PER_INT_MINUS_ONE = VALUES_PER_INT - 1;
        constexpr static u32 MAX_VALUE = 1 << BITS_PER_VALUE - 1;
        constexpr static u32 TOTAL_INTS = (SIZE * BITS_PER_VALUE + 31) >> 5;
        constexpr static u32 BIT_MASK = 0xFFFFFFFF >> (32 - BITS_PER_VALUE);

        u32 m_data[TOTAL_INTS];
    };
} // namespace nith
