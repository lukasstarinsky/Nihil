#pragma once

#include "OpenGLCommon.hpp"

#ifdef NIHIL_PLATFORM_WINDOWS
inline PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
inline PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;

namespace OpenGLLoader
{
    void LoadWGLFunctions();
}
#elifdef NIHIL_PLATFORM_LINUX
#elifdef NIHIL_PLATFORM_APPLE
#endif

inline PFNGLCREATESHADERPROC glCreateShader;

namespace OpenGLLoader
{
    void LoadGLFunctions();
}

#define RESOLVE_GL_FUNCTION(name) name = std::bit_cast<typeof(name)>(GET_PROC_ADDRESS(#name))