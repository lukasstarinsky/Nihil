#pragma once

#include <vector>
#include "Core/Defines.hpp"

struct VulkanContext
{
    VkSurfaceKHR Surface;
    VkInstance Instance;

#ifndef NDEBUG
    VkDebugUtilsMessengerEXT DebugMessenger;
#endif
};

struct VulkanDevice
{
    VkDevice LogicalDevice;
    VkPhysicalDevice PhysicalDevice;
    VkPhysicalDeviceProperties DeviceProperties;
    VkPhysicalDeviceFeatures DeviceFeatures;

    VkSurfaceCapabilitiesKHR Capabilities;
    std::vector<VkSurfaceFormatKHR> SurfaceFormats;
    std::vector<VkPresentModeKHR> PresentModes;

    // NOTE: intended wrap
    u32 GraphicsQueueFamilyIndex = -1;
    u32 PresentQueueFamilyIndex = -1;
    VkQueue GraphicsQueue;
    VkQueue PresentQueue;
};