#include "OpenGLLoader.hpp"
#include "Platform/Platform.hpp"

#ifdef NIHIL_PLATFORM_WINDOWS
void OpenGLLoader::LoadWGLFunctions()
{
    const auto& platformState = Platform::GetState();

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

    RESOLVE_GL_FUNCTION(wglCreateContextAttribsARB);
    RESOLVE_GL_FUNCTION(wglChoosePixelFormatARB);

    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(dummyContext);
}
#endif

void OpenGLLoader::LoadGLFunctions()
{

}