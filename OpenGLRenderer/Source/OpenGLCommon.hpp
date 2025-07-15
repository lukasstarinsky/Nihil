#pragma once

#ifdef NIHIL_PLATFORM_WINDOWS
#define GET_PROC_ADDRESS wglGetProcAddress
#include "Platform/Win32/PlatformWin32.hpp"
#include <GL/gl.h>
#include <wglext.h>
#elifdef NIHIL_PLATFORM_LINUX
#elifdef NIHIL_PLATFORM_APPLE
#endif

#include <glext.h>

#include "Graphics/RendererBackend.hpp"

template <typename... Args>
inline void Ensure(bool predicate, std::format_string<Args...> msg, Args&&... args)
{
    if (!predicate)
    {
        throw std::runtime_error("OpenGL error: " + std::format(msg, std::forward<Args>(args)...));
    }
}