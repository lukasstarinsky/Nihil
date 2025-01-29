#include "VulkanSwapchain.hpp"

VulkanSwapchain::VulkanSwapchain(const VulkanContext& context, const VulkanDevice& device)
    : mDevice{&device}
{
    VkSurfaceFormatKHR selectedSurfaceFormat = device.SurfaceFormats[0];
    for (const auto& surfaceFormat: device.SurfaceFormats)
    {
        if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM && surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            selectedSurfaceFormat = surfaceFormat;
            break;
        }
    }

    VkPresentModeKHR selectedPresentMode = VK_PRESENT_MODE_FIFO_KHR;
    for (auto presentMode: device.PresentModes)
    {
        if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            selectedPresentMode = presentMode;
            break;
        }
    }

    u32 imageCount = device.Capabilities.minImageCount + 1;
    if (device.Capabilities.maxImageCount > 0 && device.Capabilities.maxImageCount < imageCount)
    {
        imageCount = device.Capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapChainCreateInfo {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .surface = context.Surface,
        .minImageCount = imageCount,
        .imageFormat = selectedSurfaceFormat.format,
        .imageColorSpace = selectedSurfaceFormat.colorSpace,
        .imageExtent = device.Capabilities.currentExtent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
        .preTransform = device.Capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = selectedPresentMode,
        .clipped = VK_TRUE,
        .oldSwapchain = nullptr
    };

    u32 queueFamilyIndices[] { device.GraphicsQueueFamilyIndex, device.PresentQueueFamilyIndex };
    if (queueFamilyIndices[0] != queueFamilyIndices[1])
    {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapChainCreateInfo.queueFamilyIndexCount = COUNT_OF(queueFamilyIndices);
        swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }

    VK_CHECK(vkCreateSwapchainKHR(device.LogicalDevice, &swapChainCreateInfo, nullptr, &this->Swapchain));

    u32 swapchainImageCount;
    VK_CHECK(vkGetSwapchainImagesKHR(device.LogicalDevice, this->Swapchain, &swapchainImageCount, nullptr));

    this->Images.resize(imageCount);
    this->ImageViews.resize(imageCount);
    VK_CHECK(vkGetSwapchainImagesKHR(device.LogicalDevice, this->Swapchain, &imageCount, this->Images.data()));

    for (std::size_t i = 0; i < this->ImageViews.size(); ++i)
    {
        VkImageViewCreateInfo createInfo {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = this->Images[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = selectedSurfaceFormat.format,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        };

        VK_CHECK(vkCreateImageView(device.LogicalDevice, &createInfo, nullptr, &this->ImageViews[i]));
    }
}

VulkanSwapchain::~VulkanSwapchain()
{
    for (auto imageView: this->ImageViews)
    {
        vkDestroyImageView(mDevice->LogicalDevice, imageView, nullptr);
    }
    vkDestroySwapchainKHR(mDevice->LogicalDevice, this->Swapchain, nullptr);
}