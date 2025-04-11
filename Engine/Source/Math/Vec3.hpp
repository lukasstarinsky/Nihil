#pragma once

#include <array>

#include "Core/Defines.hpp"

class Vec3f
{
public:
    union
    {
        struct { f32 x, y, z; };
        struct { f32 r, g, b; };
        struct { f32 u, v, w; };
    };

    constexpr Vec3f()
        : x{0.0f}, y{0.0f}, z{0.0f}
    {

    }

    constexpr Vec3f(f32 x, f32 y, f32 z)
        : x{x}, y{y}, z{z}
    {

    }

    constexpr void Normalize()
    {
        auto mag = std::hypot(x, y, z);
        if (mag == 0.0f)
            return;

        x /= mag;
        y /= mag;
        z /= mag;
    }

    constexpr auto operator-(const Vec3f& other) const -> Vec3f
    {
        Vec3f out;
        out.x = x - other.x;
        out.y = y - other.y;
        out.z = z - other.z;
        return out;
    }

    constexpr auto operator-() const -> Vec3f
    {
        return {-x, -y, -z};
    }

    constexpr auto operator*(f32 scalar) const -> Vec3f
    {
        Vec3f out;
        out.x = x * scalar;
        out.y = y * scalar;
        out.z = z * scalar;
        return out;
    }

    constexpr auto operator+=(const Vec3f& other) -> Vec3f&
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
public:
    static constexpr auto Normalize(Vec3f vec) -> Vec3f
    {
        vec.Normalize();
        return vec;
    }

    static constexpr auto Cross(const Vec3f& a, const Vec3f& b) -> Vec3f
    {
        return {a.y * b.z - a.z*b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
    }

    static constexpr auto Dot(const Vec3f& a, const Vec3f& b) -> f32
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
};

template <>
class std::formatter<Vec3f>
{
public:
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    template <typename Context>
    auto format(const Vec3f& v, Context& ctx) const
    {
        return std::format_to(ctx.out(), "Vector3[X:{:.2f}, Y:{:.2f}, Z:{:.2f}]", v.x, v.y, v.z);
    }
};