#pragma once

namespace VulkanPlatform
{
    auto GetSurfaceExtension() -> const char*;
    auto CreateSurface(VkInstance vkInstance) -> VkSurfaceKHR;
}