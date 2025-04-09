#pragma once

#include <array>
#include "Core/Defines.hpp"
#include "Vec3.hpp"

template <typename T>
class Mat4
{
public:
    constexpr Mat4() = default;

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