#include <gtest/gtest.h>

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

TEST(Mat4, RotateX)
{
    auto actual = Mat4f::RotateX(1.0f);
    auto expected = Mat4f({1.0000f, 0.0000f, 0.0000f, 0.0000f, 0.0000f, 0.5403f, 0.8415f, 0.0000f, 0.0000f, -0.8415f, 0.5403f, 0.0000f, 0.0000f, 0.0000f, 0.0000f, 1.0000f});

    for (i32 i = 0; i < 16; ++i)
    {
        ASSERT_NEAR(actual[i], expected[i], 0.001f);
    }
}

TEST(Mat4, RotateY)
{
    auto actual = Mat4f::RotateY(1.0f);
    auto expected = Mat4f({0.5403f, 0.0000f, -0.8415f, 0.0000f, 0.0000f, 1.0000f, 0.0000f, 0.0000f, 0.8415f, 0.0000f, 0.5403f, 0.0000f, 0.0000f, 0.0000f, 0.0000f, 1.0000f});

    for (i32 i = 0; i < 16; ++i)
    {
        ASSERT_NEAR(actual[i], expected[i], 0.001f);
    }
}

TEST(Mat4, RotateZ)
{
    auto actual = Mat4f::RotateZ(1.0f);
    auto expected = Mat4f({0.5403f, 0.8415f, 0.0000f, 0.0000f, -0.8415f, 0.5403f, 0.0000f, 0.0000f, 0.0000f, 0.0000f, 1.0000f, 0.0000f, 0.0000f, 0.0000f, 0.0000f, 1.0000f});

    for (i32 i = 0; i < 16; ++i)
    {
        ASSERT_NEAR(actual[i], expected[i], 0.001f);
    }
}

TEST(Mat4, Multiply)
{
    auto a = Mat4f({1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f});
    auto b = Mat4f({1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.0f, 2.0f, 3.0f, 4.0f});
    auto actual = a * b;
    auto expected = Mat4f({10.0000f, 20.0000f, 30.0000f, 40.0000f, 10.0000f, 20.0000f, 30.0000f, 40.0000f, 10.0000f, 20.0000f, 30.0000f, 40.0000f, 10.0000f, 20.0000f, 30.0000f, 40.0000f});

    for (i32 i = 0; i < 16; ++i)
    {
        ASSERT_NEAR(actual[i], expected[i], 0.001f);
    }
}