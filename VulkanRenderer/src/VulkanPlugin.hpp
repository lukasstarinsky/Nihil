#pragma once

#include <Renderer/RendererPlugin.hpp>
#include "VulkanCommon.hpp"

class VulkanPlugin : public RendererPlugin
{
public:
    explicit VulkanPlugin(i32 width, i32 height);
    ~VulkanPlugin() override;
private:
    bool DeviceMeetsRequirements(VkPhysicalDevice device);
private:
    VulkanContext mContext {};
    VulkanDevice mDevice {};
    VkSwapchainKHR mSwapChain {};
};

extern "C"
{
    NIHIL_API auto CreatePlugin(i32 width, i32 height) -> RendererPlugin*;
    NIHIL_API void DestroyPlugin(RendererPlugin* plugin);
}