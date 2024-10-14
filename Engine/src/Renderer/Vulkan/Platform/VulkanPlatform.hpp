#pragma once

#include "Renderer/Vulkan/VulkanCommon.hpp"

namespace VulkanPlatform
{
    auto GetSurfaceExtension() -> const char*;
    auto CreateSurface(VkInstance vkInstance) -> VkSurfaceKHR;
}