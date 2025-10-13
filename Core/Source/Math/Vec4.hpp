#pragma once

#include "Common/Defines.hpp"

class Vec4f
{
public:
    union
    {
        struct { f32 x, y, z, w; };
        struct { f32 r, g, b, a; };
        struct { f32 u, v, s, t; };
    };

    constexpr Vec4f()
        : x{0.0f}, y{0.0f}, z{0.0f}, w{0.0f}
    {

    }

    constexpr Vec4f(f32 x, f32 y, f32 z, f32 w)
        : x{x}, y{y}, z{z}, w{w}
    {

    }
};

template <>
class std::formatter<Vec4f>
{
public:
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    template <typename Context>
    auto format(const Vec4f& v, Context& ctx) const
    {
        return std::format_to(ctx.out(), "Vector4[X:{:.2f}, Y:{:.2f}, Z:{:.2f}, W:{:.2f}]", v.x, v.y, v.z, v.w);
    }
};
