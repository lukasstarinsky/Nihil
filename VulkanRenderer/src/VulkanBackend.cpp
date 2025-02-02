#include "VulkanBackend.hpp"

VulkanBackend::VulkanBackend(const ApplicationConfig& config)
    : mContext{config}
    , mDevice{mContext}
    , mSwapchain{mContext, mDevice}
{
}

VulkanBackend::~VulkanBackend()
{
}

auto VulkanBackend::GetType() const -> RendererAPI
{
    return RendererAPI::Vulkan;
}

auto VulkanBackend::GetTypeString() const -> const char*
{
    return "Vulkan";
}

// TODO: This works, but exceptions propagated from extern "C" functions is probably undefined behavior, investigate work arounds
extern "C"
{
    NIHIL_API auto CreatePlugin(const ApplicationConfig& appConfig) -> RendererBackend*
    {
        Logger::Trace("Initializing Vulkan renderer...");
        return new VulkanBackend(appConfig);
    }

    NIHIL_API void DestroyPlugin(VulkanBackend* plugin)
    {
        Logger::Trace("Shutting down Vulkan renderer...");
        delete plugin;
    }
}