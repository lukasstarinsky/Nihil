#pragma once

#include "Vec2.hpp"

struct Rect
{
    Rect(const Vec2f& position = {}, const Vec2f& size = {})
        : Position{position}
        , Size{size}
    {
    }

    Rect(f32 x, f32 y, f32 width, f32 height)
        : Position{x, y}
        , Size{width, height}
    {
    }

    auto Contains(const Vec2f& point) const -> bool
    {
        return point.x >= Position.x && point.x <= Position.x + Size.x &&
               point.y >= Position.y && point.y <= Position.y + Size.y;
    }

    Vec2f Position;
    Vec2f Size;
};

template <>
class std::formatter<Rect>
{
public:
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    template <typename Context>
    auto format(const Rect& r, Context& ctx) const
    {
        return std::format_to(ctx.out(), "Rect(Position: {}, Size: {})", r.Position, r.Size);
    }
};
