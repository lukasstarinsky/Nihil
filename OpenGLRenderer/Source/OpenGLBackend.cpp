#include "OpenGLBackend.hpp"
#include "OpenGLShader.hpp"
#include "Platform/Platform.hpp"

std::shared_ptr<OpenGLShader> vert {};
std::shared_ptr<OpenGLShader> frag {};
GLuint program {};
GLuint vao {};
GLuint vbo {};

f32 vertexData[] = {
    0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
};

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

    // TODO: temp, abstract
    vert = std::make_shared<OpenGLShader>("Assets/Shaders/test.vert", ShaderType::Vertex);
    frag = std::make_shared<OpenGLShader>("Assets/Shaders/test.frag", ShaderType::Fragment);
    GL_CHECK(program = glCreateProgram());

    GL_CHECK(glAttachShader(program, vert->mHandle));
    GL_CHECK(glAttachShader(program, frag->mHandle));
    GL_CHECK(glLinkProgram(program));

    GL_CHECK(glGenVertexArrays(1, &vao));
    GL_CHECK(glGenBuffers(1, &vbo));

    GL_CHECK(glBindVertexArray(vao));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW));

    GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), reinterpret_cast<void*>(0)));
    GL_CHECK(glEnableVertexAttribArray(0));
    GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), reinterpret_cast<void*>(3 * sizeof(f32))));
    GL_CHECK(glEnableVertexAttribArray(1));
    glBindVertexArray(0);
}

OpenGLBackend::~OpenGLBackend()
{
    if (program)
    {
        glDeleteProgram(program);
    }
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GL_CHECK(glUseProgram(program));
    GL_CHECK(glBindVertexArray(vao));
    GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 3));
}

void OpenGLBackend::EndFrame() const
{
    const auto& platformState = Platform::GetState();

#ifdef NIHIL_PLATFORM_WINDOWS
    ::SwapBuffers(platformState.DeviceContext);
#endif
}

auto OpenGLBackend::CreateShader(const std::string& filePath, ShaderType shaderType) const -> std::shared_ptr<Shader>
{
    return std::make_shared<OpenGLShader>(filePath, shaderType);
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