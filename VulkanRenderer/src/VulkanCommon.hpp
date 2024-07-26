#pragma once

#include <Nihil.hpp>
#include <vulkan/vulkan.h>
#include "VulkanException.hpp"
#include "VulkanTypes.hpp"

#define VK_CHECK(fun, message)  \
do {                            \
    VkResult res = fun;         \
    if (res != VK_SUCCESS)      \
        VK_THROW(message, res); \
} while (false)