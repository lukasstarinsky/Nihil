#pragma once

#include <array>
#include "Core/Defines.hpp"

template <typename T>
class Vec2
{
public:
    union
    {
        struct { T x, y; };
        struct { T r, g; };
        struct { T u, v; };
        std::array<T, 2> elements {};
    };

    constexpr Vec2() = default;

    constexpr Vec2(T x, T y)
        : x{x}, y{y}
    {

    }
};

template <typename T>
class std::formatter<Vec2<T>>
{
public:
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    template <typename Context>
    auto format(const Vec2<T>& v, Context& ctx) const
    {
        if constexpr (std::same_as<T, f32>)
            return std::format_to(ctx.out(), "Vector2[X:{:.2f}, Y:{:.2f}]", v.x, v.y);
        else
            return std::format_to(ctx.out(), "Vector2[X:{}, Y:{}]", v.x, v.y);
    }
};

using Vec2i = Vec2<i32>;
using Vec2f = Vec2<f32>;