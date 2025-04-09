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

    constexpr auto operator[](std::size_t index) -> T&
    {
        ASSERT(index >= 0 && index < mElements.size());
        return mElements[index];
    }

    constexpr auto operator[](std::size_t index) const -> const T&
    {
        ASSERT(index >= 0 && index < mElements.size());
        return mElements[index];
    }

    auto Data() const -> const T*
    {
        return mElements.data();
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

    // http://www.songho.ca/opengl/gl_projectionmatrix.html#perspective
    static constexpr auto Perspective(f32 fov, f32 aspectRatio, f32 nearPlane, f32 farPlane) -> Mat4<f32>
    {
        const auto tmp = std::tan(fov / 2.0f);
        const auto t = tmp * nearPlane;
        const auto b = -t;
        const auto r = t * aspectRatio;
        const auto l = b * aspectRatio;

        Mat4<f32> out;
        out.mElements = {
            (2.0f * nearPlane) / (r - l), 0.0f, 0.0f, 0.0f,
            0.0f, (2.0f * nearPlane) / (t - b), 0.0f, 0.0f,
            (r + l) / (r - l), (t + b) / (t - b), -(farPlane + nearPlane) / (farPlane - nearPlane), -1.0f,
            0.0f, 0.0f, -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane), 0.0f
        };
        return out;
    }

    // http://www.songho.ca/opengl/gl_lookattoaxes.html
    static constexpr auto LookAt(const Vec3f& eye, const Vec3f& lookAt, const Vec3f& up) -> Mat4<f32>
    {
        const auto upNormalized = Vec3f::Normalize(up);
        const auto f = Vec3f::Normalize(lookAt - eye);
        const auto l = Vec3f::Normalize(Vec3f::Cross(f, upNormalized));
        const auto u = Vec3f::Normalize(Vec3f::Cross(l, f));

        Mat4<f32> out;
        out[0] = l.x;
        out[4] = l.y;
        out[8] = l.z;
        out[1] = u.x;
        out[5] = u.y;
        out[9] = u.z;
        out[2] = -f.x;
        out[6] = -f.y;
        out[10] = -f.z;
        out[12] = -Vec3f::Dot(l, eye);
        out[13] = -Vec3f::Dot(u, eye);
        out[14] = Vec3f::Dot(f, eye);
        out[15] = 1.0f;
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