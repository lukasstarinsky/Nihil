#include "Renderer.hpp"
#include "RendererPlugin.hpp"
#include "Platform/Platform.hpp"

static DynamicLibrary sRendererModule;
static RendererPlugin* sRendererPlugin;

void Renderer::Initialize(const ApplicationConfig& config)
{
    if (!DynamicLibrary::Load(Renderer::ApiToString(config.RendererAPI), &sRendererModule))
    {
        NTHROW(std::format("Failed to load '{}' dynamic library.", Renderer::ApiToString(config.RendererAPI)));
    }

    if (!sRendererModule.LoadFunction("CreatePlugin") || !sRendererModule.LoadFunction("DestroyPlugin"))
    {
        NTHROW(std::format("Failed to load functions of module '{}'.", Renderer::ApiToString(config.RendererAPI)));
    }

    auto CreatePluginFn { sRendererModule.GetFunction<CreateRendererPluginFn>("CreatePlugin") };
    sRendererPlugin = CreatePluginFn(config.WindowWidth, config.WindowHeight);
}

void Renderer::Shutdown()
{
    // TODO: investigate occasional crash when deleting object allocated from dll
    delete sRendererPlugin;
    sRendererModule.Unload();
}

const char* Renderer::ApiToString(RendererAPI api)
{
    switch (api)
    {
        case RendererAPI::Vulkan:     return "VulkanRenderer";
        case RendererAPI::OpenGL:     return "OpenGLRenderer";
        case RendererAPI::Direct3D11: return "Direct3D11Renderer";
        case RendererAPI::Direct3D12: return "Direct3D12Renderer";
        case RendererAPI::Metal:      return "MetalRenderer";
    }
}