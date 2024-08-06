#pragma once

#include <Nihil.hpp>
#include <vulkan/vulkan.h>
#include "VulkanException.hpp"
#include "VulkanTypes.hpp"

#define VK_CHECK(fun, ...)                              \
do {                                                    \
    VkResult res = fun(__VA_ARGS__);                    \
    if (res != VK_SUCCESS)                              \
        VK_THROW(TO_STR(fun failed with reason:), res); \
} while (false)