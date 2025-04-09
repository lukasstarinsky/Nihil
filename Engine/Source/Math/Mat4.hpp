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

using Mat4i = Mat4<i32>;
using Mat4f = Mat4<f32>;