#pragma once

#include <Core/Defines.hpp>
#include <optional>

struct VulkanQueueFamilyIndex
{
    std::optional<u32> Graphics;
    std::optional<u32> Present;

    void Reset() { Graphics = std::nullopt; Present = std::nullopt; }
    bool IsComplete() const { return Graphics.has_value() && Present.has_value(); }
};

struct VulkanSwapChainSupportInfo
{
    VkSurfaceCapabilitiesKHR Capabilities;
    std::vector<VkSurfaceFormatKHR> SurfaceFormats;
    std::vector<VkPresentModeKHR> PresentModes;
};

struct VulkanDevice
{
    VkPhysicalDevice PhysicalDevice;
    VkPhysicalDeviceProperties DeviceProps;
    VkPhysicalDeviceFeatures DeviceFeatures;
    VkPhysicalDeviceMemoryProperties DeviceMemoryProps;
    VkDevice LogicalDevice;

    VulkanQueueFamilyIndex QueueFamilyIndex;

    VkQueue GraphicsQueue;
    VkQueue PresentQueue;
};