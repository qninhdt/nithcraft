#pragma once

#include <vector>
#include <stack>
#include <queue>
#include <string>
#include <array>

#include <robin_hood.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#ifdef _MSC_VER
    #define NITH_UNUSED [[maybe_unused]]
#else
    #define NITH_UNUSED [[gnu::unused]]
#endif

#define NITH_STATIC_ERROR(message) static_assert(nith::internal::always_false<T> && message);

namespace nith
{
    namespace internal
    {
        template <class...>
        constexpr std::false_type always_false{};
    };

    using i8 = std::int8_t;
    using i16 = std::int16_t;
    using i32 = std::int32_t;
    using i64 = std::int64_t;

    using u8 = std::uint8_t;
    using u16 = std::uint16_t;
    using u32 = std::uint32_t;
    using u64 = std::uint64_t;

    using f32 = float;
    using f64 = double;

    using vec2 = glm::vec2;
    using vec3 = glm::vec3;
    using vec4 = glm::vec4;

    using ivec2 = glm::ivec2;
    using ivec3 = glm::ivec3;
    using ivec4 = glm::ivec4;

    using uvec2 = glm::uvec2;
    using uvec3 = glm::uvec3;
    using uvec4 = glm::uvec4;

    using mat2 = glm::mat2;
    using mat3 = glm::mat3;
    using mat4 = glm::mat4;

    using string = std::string;

    template <typename K, typename V>
    using umap = robin_hood::unordered_map<K, V>;

    template <typename V>
    using uset = robin_hood::unordered_set<V>;

    template <typename V, u32 size>
    using array = std::array<V, size>;
    
    template <typename V>
    using stack = std::stack<V>;

    template <typename... V>
    using tuple = std::tuple<V...>;

    template <typename V>
    using queue = std::queue<V>;

    template <typename V>
    using vector = std::vector<V>;

    constexpr double pi = 3.14159265358979323846;

} // namespace nith
