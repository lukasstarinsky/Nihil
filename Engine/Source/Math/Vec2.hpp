#pragma once

#include <array>
#include "Core/Defines.hpp"

template <typename T>
class Vec2
{
public:
    union
    {
        struct { T x, y; };
        struct { T r, g; };
        struct { T u, v; };
        std::array<T, 2> elements {};
    };

    Vec2() = default;

    Vec2(T x, T y)
        : x{x}, y{y}
    {

    }
};

using Vec2i = Vec2<i32>;
using Vec2f = Vec2<f32>;