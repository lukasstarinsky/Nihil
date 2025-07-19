#pragma once

#ifdef NIHIL_PLATFORM_WINDOWS
#include "Platform/Win32/PlatformWin32.hpp"
#elifdef NIHIL_PLATFORM_LINUX
#elifdef NIHIL_PLATFORM_APPLE
#endif

#include <vulkan/vulkan.h>

#include "Graphics/RendererBackend.hpp"

template <typename... Args>
inline void Ensure(bool predicate, std::format_string<Args...> msg, Args&&... args)
{
    if (!predicate)
    {
        throw std::runtime_error("Vulkan error: " + std::format(msg, std::forward<Args>(args)...));
    }
}