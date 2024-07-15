#pragma once

#include <Renderer/RendererPlugin.hpp>
#include "VulkanCommon.hpp"

class VulkanPlugin : public RendererPlugin
{
public:
    VulkanPlugin();
    ~VulkanPlugin() override;
private:
    bool DeviceMeetsRequirements(VkPhysicalDevice device);
private:
    VulkanDevice mDevice {};
    VkSurfaceKHR mSurface {};
    VkInstance mInstance {};
    VulkanSwapChainSupportInfo mSwapChainSupport {};

#ifndef NDEBUG
    VkDebugUtilsMessengerEXT mDebugMessenger {};
#endif
};

extern "C"
{
    NIHIL_API auto CreatePlugin() -> RendererPlugin*;
    NIHIL_API void DestroyPlugin(RendererPlugin* plugin);
}