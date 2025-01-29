#pragma once

#include "VulkanCommon.hpp"
#include "VulkanContext.hpp"

class VulkanDevice
{
public:
    explicit VulkanDevice(const VulkanContext& context);
    ~VulkanDevice();
private:
    auto IsDeviceSuitable(const VkPhysicalDevice& device, VkSurfaceKHR surface) -> bool;
public:
    VkDevice LogicalDevice {};
    VkPhysicalDevice PhysicalDevice {};

    VkSurfaceCapabilitiesKHR Capabilities {};
    std::vector<VkSurfaceFormatKHR> SurfaceFormats;
    std::vector<VkPresentModeKHR> PresentModes;

    // NOTE: intended wrap
    u32 GraphicsQueueFamilyIndex = -1;
    u32 PresentQueueFamilyIndex = -1;
    VkQueue GraphicsQueue {};
    VkQueue PresentQueue {};
};
