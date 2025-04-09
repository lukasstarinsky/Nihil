#include <gtest/gtest.h>

#include "Math/Math.hpp"

TEST(Vec3, Normalize)
{
    auto actual = Vec3f::Normalize({1.0f, 2.0f, 3.0f});
    auto expected = Vec3f{0.2673, 0.5345, 0.8018};

    ASSERT_NEAR(actual.x, expected.x, 0.001f);
    ASSERT_NEAR(actual.y, expected.y, 0.001f);
    ASSERT_NEAR(actual.z, expected.z, 0.001f);
}

TEST(Vec3, Cross)
{
    auto actual = Vec3f::Cross({1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f});
    auto expected = Vec3f{-3.0000, 6.0000, -3.0000};

    ASSERT_NEAR(actual.x, expected.x, 0.001f);
    ASSERT_NEAR(actual.y, expected.y, 0.001f);
    ASSERT_NEAR(actual.z, expected.z, 0.001f);
}

TEST(Vec3, Dot)
{
    auto actual = Vec3f::Dot({1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f});
    auto expected = 32.00f;

    ASSERT_NEAR(actual, expected, 0.001f);
}