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

    constexpr auto operator-(const Vec3<T>& other) const -> Vec3<T>
    {
        Vec3<T> out;
        out.x = x - other.x;
        out.y = y - other.y;
        out.z = z - other.z;
        return out;
    }
public:
    static constexpr auto Normalize(Vec3<T> v) -> Vec3<T>
    {
        auto mag = std::hypot(v.x, v.y, v.z);
        if (mag == 0.0f)
            return v;

        v.x /= mag;
        v.y /= mag;
        v.z /= mag;
        return v;
    }

    static constexpr auto Cross(const Vec3<T>& a, const Vec3<T>& b) -> Vec3<T>
    {
        return {a.y * b.z - a.z*b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
    }

    static constexpr auto Dot(const Vec3<T>& a, const Vec3<T>& b) -> T
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
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