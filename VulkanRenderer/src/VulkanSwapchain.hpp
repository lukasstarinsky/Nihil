#pragma once

#include "VulkanCommon.hpp"
#include "VulkanDevice.hpp"
#include "VulkanContext.hpp"

class VulkanSwapchain
{
public:
    VulkanSwapchain(const VulkanContext& context, const VulkanDevice& device);
    ~VulkanSwapchain();
private:
    VkSwapchainKHR mSwapchain {};
    VkDevice mDevice {};
    std::vector<VkImage> mImages;
    std::vector<VkImageView> mImageViews;
};