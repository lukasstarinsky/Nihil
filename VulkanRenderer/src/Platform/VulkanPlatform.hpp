#pragma once

#include "VulkanCommon.hpp"

namespace VulkanPlatform
{
    const char* GetSurfaceExtension();
    VkSurfaceKHR CreateSurface(VkInstance vkInstance);
}