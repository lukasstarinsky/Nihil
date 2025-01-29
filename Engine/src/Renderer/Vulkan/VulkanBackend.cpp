#include "VulkanBackend.hpp"

VulkanBackend::VulkanBackend(const ApplicationConfig& config)
    : mContext{config}
    , mDevice{mContext}
    , mSwapchain{mContext, mDevice}
{
    Logger::Trace("Initializing Vulkan renderer...");
}

VulkanBackend::~VulkanBackend()
{
    Logger::Trace("Shutting down Vulkan renderer...");
}