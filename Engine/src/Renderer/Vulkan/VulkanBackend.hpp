#pragma once

#include "Renderer/Renderer.hpp"
#include "VulkanCommon.hpp"

class VulkanBackend : public RendererBackend
{
public:
    explicit VulkanBackend(i32 width, i32 height);
    ~VulkanBackend() override;
private:
    bool DeviceMeetsRequirements(VkPhysicalDevice device);
private:
    VulkanContext mContext {};
    VulkanDevice mDevice {};
    VkSwapchainKHR mSwapChain {};
};