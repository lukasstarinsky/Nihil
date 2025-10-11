#include <gtest/gtest.h>

#include "Platform/UUID.hpp"

TEST(UUID, Compare)
{
    auto uuid1 = Nihil::UUID::Generate();
    auto uuid2 = Nihil::UUID::Generate();
    auto uuid3 = uuid1;

    ASSERT_FALSE(uuid1 == uuid2);
    ASSERT_TRUE(uuid1 == uuid3);
}

TEST(UUID, Stringify)
{
    auto uuid1 = Nihil::UUID::Generate();
    auto str1 = uuid1.ToString();
    auto uuid2 = Nihil::UUID::FromString(str1);
    auto str2 = uuid2.ToString();

    ASSERT_TRUE(uuid1 == uuid2);
    ASSERT_EQ(str1, str2);
}