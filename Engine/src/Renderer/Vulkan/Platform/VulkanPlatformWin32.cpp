#define VK_USE_PLATFORM_WIN32_KHR
#include "VulkanPlatform.hpp"
#include "Platform/Platform.hpp"

struct PlatformState
{
    HWND WindowHandle;
    HINSTANCE Instance;
};

auto VulkanPlatform::GetSurfaceExtension() -> const char*
{
    return VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
}

auto VulkanPlatform::CreateSurface(VkInstance vkInstance) -> VkSurfaceKHR
{
    auto* platformState = static_cast<PlatformState*>(Platform::GetState());

    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo {
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .hinstance = platformState->Instance,
        .hwnd = platformState->WindowHandle
    };

    VkSurfaceKHR outSurface;
    VK_CHECK(vkCreateWin32SurfaceKHR(vkInstance, &surfaceCreateInfo, nullptr, &outSurface));

    return outSurface;
}