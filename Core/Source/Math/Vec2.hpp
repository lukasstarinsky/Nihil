#pragma once

#include <array>
#include "Common/Defines.hpp"

class Vec2f
{
public:
    union
    {
        struct { f32 x, y; };
        struct { f32 r, g; };
        struct { f32 u, v; };
    };

    constexpr Vec2f()
        : x{0.0f}, y{0.0f}
    {

    }

    constexpr Vec2f(f32 x, f32 y)
        : x{x}, y{y}
    {

    }

    constexpr auto operator*(f32 scalar) const -> Vec2f
    {
        Vec2f out = *this;
        out *= scalar;
        return out;
    }

    constexpr auto operator*=(f32 scalar) -> Vec2f&
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }
};

template <>
class std::formatter<Vec2f>
{
public:
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    template <typename Context>
    auto format(const Vec2f& v, Context& ctx) const
    {
        return std::format_to(ctx.out(), "Vector2[X:{:.2f}, Y:{:.2f}]", v.x, v.y);
    }
};