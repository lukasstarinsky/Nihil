#pragma once

#include <vulkan/vulkan.h>

#include "VulkanException.hpp"

#define VK_CHECK(fun)       \
do {                        \
    VkResult res = fun;     \
    if (res != VK_SUCCESS)  \
        VK_THROW(res);      \
} while (false)