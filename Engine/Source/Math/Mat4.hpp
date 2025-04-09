#pragma once

#include <array>
#include "Core/Logger.hpp"
#include "Core/Defines.hpp"
#include "Vec3.hpp"

template <typename T>
class Mat4
{
public:
    constexpr Mat4() = default;

    constexpr Mat4(const std::array<T, 16>& elements)
        : mElements{elements}
    {

    }

    auto Data() const -> const T*
    {
        return mElements.data();
    }

    auto operator[](std::size_t index) const -> T
    {
        ASSERT(index >= 0 && index < mElements.size());
        return mElements[index];
    }
public:
    static constexpr auto Identity() -> Mat4<T>
    {
        Mat4<T> out;
        out.mElements[0] = 1;
        out.mElements[5] = 1;
        out.mElements[10] = 1;
        out.mElements[15] = 1;
        return out;
    }

    static constexpr auto Translation(const Vec3<T>& translation) -> Mat4<T>
    {
        Mat4<T> out = Identity();
        out.mElements[12] = translation.x;
        out.mElements[13] = translation.y;
        out.mElements[14] = translation.z;
        return out;
    }

    static constexpr auto Perspective(f32 fov, f32 aspectRatio, f32 nearPlane, f32 farPlane) -> Mat4<f32>
    {
        Mat4<f32> out;

        const auto tmp = std::tan(fov / 2.0f);
        const auto t = tmp * nearPlane;
        const auto b = -t;
        const auto r = t * aspectRatio;
        const auto l = b * aspectRatio;

        out.mElements = {
            (2.0f * nearPlane) / (r - l), 0.0f, 0.0f, 0.0f,
            0.0f, (2.0f * nearPlane) / (t - b), 0.0f, 0.0f,
            (r + l) / (r - l), (t + b) / (t - b), -(farPlane + nearPlane) / (farPlane - nearPlane), -1.0f,
            0.0f, 0.0f, -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane), 0.0f
        };

        return out;
    }
private:
    std::array<T, 16> mElements {};
};

template <typename T>
class std::formatter<Mat4<T>>
{
public:
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    template <typename Context>
    auto format(const Mat4<T>& m, Context& ctx) const
    {
        return std::format_to(ctx.out(), "{} {} {} {}\n{} {} {} {}\n{} {} {} {}\n{} {} {} {}", m[0], m[4], m[8], m[12], m[1], m[5], m[9], m[13], m[2], m[6], m[10], m[14], m[3], m[7], m[11], m[15]);
    }
};

using Mat4i = Mat4<i32>;
using Mat4f = Mat4<f32>;