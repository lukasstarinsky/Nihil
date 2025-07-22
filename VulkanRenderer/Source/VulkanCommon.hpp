#pragma once

#ifdef NIHIL_PLATFORM_WINDOWS
#include "Platform/Win32/PlatformWin32.hpp"
#elifdef NIHIL_PLATFORM_LINUX
#elifdef NIHIL_PLATFORM_APPLE
#endif

#include <stacktrace>
#include <vulkan/vulkan.h>

#include "Core/Application.hpp"
#include "Graphics/RendererBackend.hpp"

template <typename... Args>
inline void Ensure(bool predicate, std::format_string<Args...> msg, Args&&... args)
{
    if (!predicate)
    {
        throw std::runtime_error(std::format("Vulkan error: {}\n{}", std::format(msg, std::forward<Args>(args)...), std::stacktrace::current(1)));
    }
}