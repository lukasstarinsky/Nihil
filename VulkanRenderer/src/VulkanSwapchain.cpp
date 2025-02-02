#include "VulkanSwapchain.hpp"

VulkanSwapchain::VulkanSwapchain(const VulkanContext& context, const VulkanDevice& device)
    : mDevice{device.GetLogicalDevice()}
{
    const auto& surfaceFormats = device.GetSurfaceFormats();
    VkSurfaceFormatKHR selectedSurfaceFormat = surfaceFormats[0];

    for (const auto& surfaceFormat: surfaceFormats)
    {
        if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM && surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            selectedSurfaceFormat = surfaceFormat;
            break;
        }
    }

    VkPresentModeKHR selectedPresentMode = VK_PRESENT_MODE_FIFO_KHR;
    for (auto presentMode: device.GetPresentModes())
    {
        if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            selectedPresentMode = presentMode;
            break;
        }
    }

    const auto& capabilities = device.GetCapabilities();
    u32 imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && capabilities.maxImageCount < imageCount)
    {
        imageCount = capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapChainCreateInfo {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .surface = context.GetSurface(),
        .minImageCount = imageCount,
        .imageFormat = selectedSurfaceFormat.format,
        .imageColorSpace = selectedSurfaceFormat.colorSpace,
        .imageExtent = capabilities.currentExtent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
        .preTransform = capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = selectedPresentMode,
        .clipped = VK_TRUE,
        .oldSwapchain = nullptr
    };

    auto queueFamilyIndices = device.GetQueueFamilyIndices();
    if (queueFamilyIndices[0] != queueFamilyIndices[1])
    {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapChainCreateInfo.queueFamilyIndexCount = COUNT_OF(queueFamilyIndices);
        swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices.data();
    }

    VK_CHECK(vkCreateSwapchainKHR(device.GetLogicalDevice(), &swapChainCreateInfo, nullptr, &mSwapchain));

    u32 swapchainImageCount;
    VK_CHECK(vkGetSwapchainImagesKHR(device.GetLogicalDevice(), mSwapchain, &swapchainImageCount, nullptr));

    mImages.resize(imageCount);
    mImageViews.resize(imageCount);
    VK_CHECK(vkGetSwapchainImagesKHR(device.GetLogicalDevice(), mSwapchain, &imageCount, mImages.data()));

    for (std::size_t i = 0; i < mImageViews.size(); ++i)
    {
        VkImageViewCreateInfo createInfo {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = mImages[i],
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

        VK_CHECK(vkCreateImageView(mDevice, &createInfo, nullptr, &mImageViews[i]));
    }
}

VulkanSwapchain::~VulkanSwapchain()
{
    for (auto imageView: mImageViews)
    {
        vkDestroyImageView(mDevice, imageView, nullptr);
    }
    vkDestroySwapchainKHR(mDevice, mSwapchain, nullptr);
}