#pragma once

#include <array>
#include "Core/Defines.hpp"

template <typename T>
class Vec3
{
public:
    union
    {
        struct { T x, y, z; };
        struct { T r, g, b; };
        struct { T u, v, w; };
        std::array<T, 3> elements {};
    };

    constexpr Vec3() = default;

    constexpr Vec3(T x, T y, T z)
        : x{x}, y{y}, z{z}
    {

    }
};

template <typename T>
class std::formatter<Vec3<T>>
{
public:
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    template <typename Context>
    auto format(const Vec3<T>& v, Context& ctx) const
    {
        if constexpr (std::same_as<T, f32>)
            return std::format_to(ctx.out(), "Vector3[X:{:.2f}, Y:{:.2f}, Z:{:.2f}]", v.x, v.y, v.z);
        else
            return std::format_to(ctx.out(), "Vector3[X:{}, Y:{}, Z: {}]", v.x, v.y, v.z);
    }
};

using Vec3i = Vec3<i32>;
using Vec3f = Vec3<f32>;