#pragma once

#ifdef NIHIL_PLATFORM_WINDOWS
#define GET_PROC_ADDRESS wglGetProcAddress
#include "Platform/Win32/Win32Platform.hpp"
#include <GL/gl.h>
#include <wglext.h>
#elifdef NIHIL_PLATFORM_LINUX
#elifdef NIHIL_PLATFORM_APPLE
#endif

#include <glext.h>

#include "Graphics/RendererBackend.hpp"