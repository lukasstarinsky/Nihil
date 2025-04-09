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

    Vec3() = default;

    Vec3(T x, T y, T z)
        : x{x}, y{y}, z{z}
    {

    }
};

using Vec3i = Vec3<i32>;
using Vec3f = Vec3<f32>;