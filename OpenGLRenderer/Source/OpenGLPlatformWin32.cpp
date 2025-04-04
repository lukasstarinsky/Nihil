#include "OpenGLPlatform.hpp"
#include "Platform/Platform.hpp"

static PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
static PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;

void OpenGLPlatform::Initialize()
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

    i32 pixelFormatAttribs[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        NULL
    };

    i32 pixelFormat = 0;
    u32 numFormats = 0;
    wglChoosePixelFormatARB(platformState.DeviceContext, pixelFormatAttribs, nullptr, 1, &pixelFormat, &numFormats);
    ENSURE(numFormats != 0, "OpenGL WGL: Failed to choose a pixel format");

    i32 attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        NULL
    };

    auto context = wglCreateContextAttribsARB(platformState.DeviceContext, nullptr, attribs);
    ENSURE(context, "OpenGL WGL: Failed to create context");
    Logger::Info("Initialized OpenGL: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(dummyContext);
    wglMakeCurrent(platformState.DeviceContext, context);
}