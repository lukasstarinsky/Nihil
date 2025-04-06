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

#include "Renderer/Renderer.hpp"
#include "OpenGLException.hpp"

#define GL_CHECK(fun) \
    while (glGetError() != GL_NO_ERROR); \
    fun; \
    if (GLenum error = glGetError()) \
        GL_THROW(error)