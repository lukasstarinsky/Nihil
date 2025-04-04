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

extern "C"
{
    NIHIL_API auto CreatePlugin(const ApplicationConfig& appConfig, std::exception_ptr& exceptionPtr) -> RendererBackend*
    {
        Logger::Trace("Initializing Vulkan renderer...");
        try
        {
            return new VulkanBackend(appConfig);
        }
        catch (...)
        {
            exceptionPtr = std::current_exception();
            return nullptr;
        }
    }

    NIHIL_API void DestroyPlugin(VulkanBackend* plugin)
    {
        Logger::Trace("Shutting down Vulkan renderer...");
        delete plugin;
    }
}