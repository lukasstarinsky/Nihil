#include "OpenGLBackend.hpp"
#include "OpenGLPlatform.hpp"

OpenGLBackend::OpenGLBackend(const ApplicationConfig& config)
{
    OpenGLPlatform::Initialize();

    ResolveGLFunctions();
}

OpenGLBackend::~OpenGLBackend()
{
}

void OpenGLBackend::ResolveGLFunctions() const
{
    RESOLVE_GL_FUNCTION(glCreateShader);
}

auto OpenGLBackend::GetType() const -> RendererAPI
{
    return RendererAPI::OpenGL;
}

auto OpenGLBackend::GetTypeString() const -> const char*
{
    return "OpenGL";
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