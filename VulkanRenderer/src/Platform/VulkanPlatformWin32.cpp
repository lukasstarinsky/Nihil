#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define VK_USE_PLATFORM_WIN32_KHR
#include "VulkanPlatform.hpp"

struct PlatformState
{
    HWND WindowHandle;
    HINSTANCE Instance;
};

const char* VulkanPlatform::GetSurfaceExtension()
{
    return "VK_KHR_win32_surface";
}

VkSurfaceKHR VulkanPlatform::CreateSurface(VkInstance vkInstance)
{
    auto* platformState { static_cast<PlatformState*>(Platform::GetState()) };

    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo {
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .hinstance = platformState->Instance,
        .hwnd = platformState->WindowHandle
    };

    VkSurfaceKHR outSurface;
    VK_CHECK(vkCreateWin32SurfaceKHR, vkInstance, &surfaceCreateInfo, nullptr, &outSurface);

    return outSurface;
}