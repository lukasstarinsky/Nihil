#include <gtest/gtest.h>
#include <print>

#include "Math/Math.hpp"

TEST(Mat4, PerspectiveMatrix)
{
    auto actual = Mat4f::Perspective(std::numbers::pi / 4.0f, 800.0f / 600.0f, 0.1f, 100.0f);
    auto expected = Mat4f({1.8107f, 0.0000f, 0.0000f, 0.0000f, 0.0000f, 2.4142f, 0.0000f, 0.0000f, 0.0000f, 0.0000f, -1.0020f, -1.0000f, 0.0000f, 0.0000f, -0.2002f, 0.0000f});

    for (i32 i = 0; i < 16; ++i)
    {
        ASSERT_NEAR(actual[i], expected[i], 0.001f);
    }
}

TEST(Mat4, LookAt)
{
    auto actual = Mat4f::LookAt({1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f}, {0.0f, 1.0f, 0.0f});
    auto expected = Mat4f({-0.7071f, -0.4082f, -0.5774f, 0.0000f, 0.0000f, 0.8165f, -0.5774f, 0.0000f, 0.7071f, -0.4082f, -0.5774f, 0.0000f, -1.4142f, -0.0000f, 3.4641f, 1.0000f});

    for (i32 i = 0; i < 16; ++i)
    {
        ASSERT_NEAR(actual[i], expected[i], 0.001f);
    }
}
