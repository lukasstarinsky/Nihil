#pragma once

#include "VulkanCommon.hpp"

namespace VulkanPlatform
{
    auto GetSurfaceExtension() -> const char*;
    auto CreateSurface(VkInstance vkInstance) -> VkSurfaceKHR;
}