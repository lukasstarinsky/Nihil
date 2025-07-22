#pragma once

#include "VulkanCommon.hpp"

class VulkanContext
{
public:
    VulkanContext(const ApplicationConfig& appConfig, const PlatformState& platformState);
    ~VulkanContext();

    void CreateInstance(const ApplicationConfig& appConfig);
    void CreateSurface(const PlatformState& platformState);
    void CreatePhysicalDevice();
    void CreateLogicalDevice();
public:
    VkInstance Instance {};
    VkSurfaceKHR Surface {};
    VkPhysicalDevice PhysicalDevice {};
    VkDevice Device {};

    VkQueue GraphicsQueue {};
    VkQueue PresentQueue {};
    u32 GraphicsQueueFamilyIndex = -1u;
    u32 PresentQueueFamilyIndex = -1u;
};