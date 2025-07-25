#pragma once

#include <array>

#include "Core/Defines.hpp"
#include "Core/Logger.hpp"
#include "Vec3.hpp"

class Mat4f
{
public:
    constexpr Mat4f()
    {
        mElements[0] = 1;
        mElements[5] = 1;
        mElements[10] = 1;
        mElements[15] = 1;
    };

    constexpr Mat4f(const std::array<f32, 16>& elements)
        : mElements{elements}
    {

    }

    constexpr Mat4f(const Vec3f& translation)
        : Mat4f{}
    {
        mElements[12] = translation.x;
        mElements[13] = translation.y;
        mElements[14] = translation.z;
    }

    constexpr auto operator[](std::size_t index) -> f32&
    {
        ASSERT(index >= 0 && index < mElements.size());
        return mElements[index];
    }

    constexpr auto operator[](std::size_t index) const -> const f32&
    {
        ASSERT(index >= 0 && index < mElements.size());
        return mElements[index];
    }

    constexpr auto operator*(const Mat4f& other) const -> Mat4f
    {
        Mat4f out;
        for (i32 col = 0; col < 4; ++col)
        {
            for (i32 row = 0; row < 4; ++row)
            {
                out[col * 4 + row] = 0.0f;
                for (i32 k = 0; k < 4; ++k)
                {
                    out[col * 4 + row] += mElements[k * 4 + row] * other[col * 4 + k];
                }
            }
        }
        return out;
    }

    auto Data() const -> const f32*
    {
        return mElements.data();
    }
public:
    static constexpr auto Identity() -> Mat4f
    {
        return {};
    }

    static constexpr auto Scale(const Vec3f& scale) -> Mat4f
    {
        Mat4f out = Identity();
        out[0] = scale.x;
        out[5] = scale.y;
        out[10] = scale.z;
        return out;
    }

    static constexpr auto RotateX(f32 theta) -> Mat4f
    {
        auto sin = std::sin(theta);
        auto cos = std::cos(theta);

        Mat4f out = Identity();
        out[5] = cos;
        out[6] = sin;
        out[9] = -sin;
        out[10] = cos;
        return out;
    }

    static constexpr auto RotateY(f32 theta) -> Mat4f
    {
        auto sin = std::sin(theta);
        auto cos = std::cos(theta);

        Mat4f out = Identity();
        out[0] = cos;
        out[2] = -sin;
        out[8] = sin;
        out[10] = cos;
        return out;
    }

    static constexpr auto RotateZ(f32 theta) -> Mat4f
    {
        auto sin = std::sin(theta);
        auto cos = std::cos(theta);

        Mat4f out = Identity();
        out[0] = cos;
        out[1] = sin;
        out[4] = -sin;
        out[5] = cos;
        return out;
    }

    static constexpr auto Translate(const Vec3f& translation) -> Mat4f
    {
        Mat4f out = Identity();
        out[12] = translation.x;
        out[13] = translation.y;
        out[14] = translation.z;
        return out;
    }

    // http://www.songho.ca/opengl/gl_projectionmatrix.html#perspective
    static constexpr auto Perspective(f32 fov, f32 aspectRatio, f32 nearPlane, f32 farPlane) -> Mat4f
    {
        const auto tmp = std::tan(fov / 2.0f);
        const auto t = tmp * nearPlane;
        const auto b = -t;
        const auto r = t * aspectRatio;
        const auto l = b * aspectRatio;

        Mat4f out = Identity();
        out[0] = (2.0f * nearPlane) / (r - l);
        out[5] = (2.0f * nearPlane) / (t - b);
        out[8] = (r + l) / (r - l);
        out[9] = (t + b) / (t - b);
        out[10] = -(farPlane + nearPlane) / (farPlane - nearPlane);
        out[11] = -1.0f;
        out[14] = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);
        out[15] = 0.0f;
        return out;
    }

    // http://www.songho.ca/opengl/gl_lookattoaxes.html
    static constexpr auto LookAt(const Vec3f& eye, const Vec3f& lookAt, const Vec3f& up) -> Mat4f
    {
        const auto upNormalized = Vec3f::Normalize(up);
        const auto f = Vec3f::Normalize(lookAt - eye);
        const auto l = Vec3f::Normalize(Vec3f::Cross(f, upNormalized));
        const auto u = Vec3f::Normalize(Vec3f::Cross(l, f));

        Mat4f out;
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
    std::array<f32, 16> mElements {};
};

template <>
class std::formatter<Mat4f>
{
public:
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    template <typename Context>
    auto format(const Mat4f& m, Context& ctx) const
    {
        return std::format_to(ctx.out(), "{} {} {} {}\n{} {} {} {}\n{} {} {} {}\n{} {} {} {}", m[0], m[4], m[8], m[12], m[1], m[5], m[9], m[13], m[2], m[6], m[10], m[14], m[3], m[7], m[11], m[15]);
    }
};