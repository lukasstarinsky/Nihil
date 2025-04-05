#include "OpenGLBackend.hpp"
#include "Platform/Platform.hpp"

OpenGLBackend::OpenGLBackend(const ApplicationConfig& config)
{
    const auto& platformState = Platform::GetState();

#ifdef NIHIL_PLATFORM_WINDOWS
    OpenGLLoader::LoadWGLFunctions();

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
    Ensure(numFormats != 0, "OpenGL WGL: Failed to choose a pixel format");

    i32 attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        NULL
    };

    auto context = wglCreateContextAttribsARB(platformState.DeviceContext, nullptr, attribs);
    Ensure(context, "OpenGL WGL: Failed to create context");

    wglMakeCurrent(platformState.DeviceContext, context);
    Logger::Info("Initialized OpenGL: {}", (char*)glGetString(GL_VERSION));
#endif

    OpenGLLoader::LoadGLFunctions();
}

OpenGLBackend::~OpenGLBackend()
{
}

auto OpenGLBackend::GetType() const -> RendererAPI
{
    return RendererAPI::OpenGL;
}

auto OpenGLBackend::GetTypeString() const -> const char*
{
    return "OpenGL";
}

void OpenGLBackend::BeginFrame(f32 r, f32 g, f32 b, f32 a) const
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLBackend::EndFrame() const
{
    const auto& platformState = Platform::GetState();

#ifdef NIHIL_PLATFORM_WINDOWS
    ::SwapBuffers(platformState.DeviceContext);
#endif
}

extern "C"
{
    NIHIL_API auto CreatePlugin(const ApplicationConfig& appConfig, std::exception_ptr& exceptionPtr) -> RendererBackend*
    {
        Logger::Trace("Initializing OpenGL renderer...");
        try
        {
            return new OpenGLBackend(appConfig);
        }
        catch (...)
        {
            exceptionPtr = std::current_exception();
            return nullptr;
        }
    }

    NIHIL_API void DestroyPlugin(OpenGLBackend* plugin)
    {
        Logger::Trace("Shutting down OpenGL renderer...");
        delete plugin;
    }
}