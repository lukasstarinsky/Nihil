#pragma once

#ifdef NIHIL_PLATFORM_WINDOWS
#define GET_PROC_ADDRESS wglGetProcAddress
#include "Platform/PlatformWin32.hpp"
#include <GL/gl.h>
#include <wglext.h>
#elifdef NIHIL_PLATFORM_LINUX
#elifdef NIHIL_PLATFORM_APPLE
#endif

#include <glext.h>

#define RESOLVE_GL_FUNCTION(name) name = std::bit_cast<typeof(name)>(GET_PROC_ADDRESS(#name))

inline PFNGLCREATESHADERPROC glCreateShader;