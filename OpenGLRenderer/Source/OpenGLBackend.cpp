#include "OpenGLBackend.hpp"

#include "Platform/Platform.hpp"
#include "OpenGLShader.hpp"
#include "OpenGLBuffer.hpp"
#include "OpenGLMaterial.hpp"
#include "OpenGLMesh.hpp"
#include "OpenGLTexture.hpp"

static void OpenGLDebugCallback([[maybe_unused]] GLenum src, [[maybe_unused]] GLenum type, [[maybe_unused]] GLuint id, GLenum severity, [[maybe_unused]] GLsizei length, const GLchar* msg, [[maybe_unused]] const void* user_param)
{
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_LOW:
        case GL_DEBUG_SEVERITY_MEDIUM:
            Logger::Warn("OpenGL message: {}", msg);
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            Throw("OpenGL error: {}", msg);
    }
}

OpenGLBackend::OpenGLBackend(const ApplicationConfig& appConfig)
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
#endif
    OpenGLLoader::LoadGLFunctions();
    Logger::Info("Initialized OpenGL: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

#ifndef NDEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLDebugCallback, nullptr);
#endif
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glViewport(0, 0, appConfig.WindowWidth, appConfig.WindowHeight);
}

OpenGLBackend::~OpenGLBackend()
{
}

auto OpenGLBackend::GetApi() const -> RendererAPI
{
    return RendererAPI::OpenGL;
}

auto OpenGLBackend::GetApiString() const -> const char*
{
    return "OpenGL";
}

void OpenGLBackend::BeginFrame(f32 r, f32 g, f32 b, f32 a) const
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLBackend::OnResize(i32 width, i32 height) const
{
    glViewport(0, 0, width, height);
}

void OpenGLBackend::EndFrame() const
{
    const auto& platformState = Platform::GetState();

#ifdef NIHIL_PLATFORM_WINDOWS
    ::SwapBuffers(platformState.DeviceContext);
#endif
}

auto OpenGLBackend::CreateShader(const ShaderSpecification& shaderSpec) const -> ShaderPtr
{
    return std::make_shared<OpenGLShader>(shaderSpec);
}

auto OpenGLBackend::CreateMaterial(const ShaderPtr& vertexShader, const ShaderPtr& fragmentShader) const -> MaterialPtr
{
    return std::make_shared<OpenGLMaterial>(vertexShader, fragmentShader);
}

auto OpenGLBackend::CreateBuffer(BufferType bufferType, const void* data, i32 size, i32 uniformBinding) const -> BufferPtr
{
    return std::make_shared<OpenGLBuffer>(bufferType, data, size, uniformBinding);
}

auto OpenGLBackend::CreateMesh(const MeshSpecification& meshSpec) const -> MeshPtr
{
    return std::make_shared<OpenGLMesh>(meshSpec);
}

auto OpenGLBackend::CreateTexture(const TextureSpecification& textureSpec) const -> TexturePtr
{
    return std::make_shared<OpenGLTexture>(textureSpec);
}

void OpenGLBackend::Draw(const MeshPtr& mesh) const
{
    mesh->Bind();
    glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
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