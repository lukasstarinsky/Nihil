#pragma once

#include "VulkanCommon.hpp"
#include "VulkanDevice.hpp"
#include "VulkanContext.hpp"

class VulkanSwapchain
{
public:
    VulkanSwapchain(const VulkanContext& context, const VulkanDevice& device);
    ~VulkanSwapchain();
public:
    VkSwapchainKHR Swapchain {};
    std::vector<VkImage> Images;
    std::vector<VkImageView> ImageViews;
private:
    const VulkanDevice* mDevice;
};