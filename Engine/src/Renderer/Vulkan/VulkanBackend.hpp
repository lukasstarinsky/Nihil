#pragma once

#include "Renderer/Renderer.hpp"
#include "VulkanCommon.hpp"

class VulkanBackend : public RendererBackend
{
public:
    explicit VulkanBackend(i32 width, i32 height);
    ~VulkanBackend() override;
private:
    auto DeviceMeetsRequirements(VkPhysicalDevice device) -> bool;
private:
    VulkanContext mContext {};
    VulkanDevice mDevice {};
    VkSwapchainKHR mSwapChain {};
};