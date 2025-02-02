#pragma once

#include <array>

#include "VulkanCommon.hpp"
#include "VulkanContext.hpp"

class VulkanDevice
{
public:
    explicit VulkanDevice(const VulkanContext& context);
    ~VulkanDevice();

    auto GetLogicalDevice() const -> VkDevice;
    auto GetSurfaceFormats() const -> const std::vector<VkSurfaceFormatKHR>&;
    auto GetPresentModes() const -> const std::vector<VkPresentModeKHR>&;
    auto GetCapabilities() const -> const VkSurfaceCapabilitiesKHR&;
    auto GetQueueFamilyIndices() const -> std::array<u32, 2>;
private:
    auto IsDeviceSuitable(const VkPhysicalDevice& device, VkSurfaceKHR surface) -> bool;
private:
    VkDevice mLogicalDevice {};
    VkPhysicalDevice mPhysicalDevice {};

    VkSurfaceCapabilitiesKHR mCapabilities {};
    std::vector<VkSurfaceFormatKHR> mSurfaceFormats;
    std::vector<VkPresentModeKHR> mPresentModes;

    // NOTE: intended wrap
    u32 mGraphicsFamilyIndex = -1;
    u32 mPresentFamilyIndex = -1;
    VkQueue mGraphicsQueue {};
    VkQueue mPresentQueue {};
};
