#include <gtest/gtest.h>

#include "Math/Math.hpp"

TEST(Rect, ContainsPoint)
{
    Rect rect(0.0f, 0.0f, 10.0f, 10.0f);
    Vec2f point(5.0f, 5.0f);
    Vec2f point2(20.0f, 20.0f);

    ASSERT_TRUE(rect.Contains(point));
    ASSERT_FALSE(rect.Contains(point2));
}