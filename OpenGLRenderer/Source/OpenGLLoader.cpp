#include "OpenGLLoader.hpp"
#include "Platform/Platform.hpp"

#define RESOLVE_GL_FUNCTION_DEFINITION(type, name) name = std::bit_cast<type>(GET_PROC_ADDRESS(#name));

#ifdef NIHIL_PLATFORM_WINDOWS
void OpenGLLoader::LoadWGLFunctions(const PlatformState& platformState)
{
    PIXELFORMATDESCRIPTOR pfd {};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    auto format = ChoosePixelFormat(platformState.DeviceContext, &pfd);
    SetPixelFormat(platformState.DeviceContext, format, &pfd);

    auto dummyContext = wglCreateContext(platformState.DeviceContext);
    wglMakeCurrent(platformState.DeviceContext, dummyContext);

    FOR_OPENGL_WGL_FUNCTIONS(RESOLVE_GL_FUNCTION_DEFINITION)

    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(dummyContext);
}
#endif

void OpenGLLoader::LoadGLFunctions()
{
    FOR_OPENGL_FUNCTIONS(RESOLVE_GL_FUNCTION_DEFINITION)
}