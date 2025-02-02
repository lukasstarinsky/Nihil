#include "Renderer.hpp"
#include "Platform/DynamicLibrary.hpp"

static DynamicLibrary sRendererModule;
static RendererBackend* sRendererBackend;

void Renderer::Initialize(const ApplicationConfig& config)
{
    if (!DynamicLibrary::Load(Renderer::ApiToModuleString(config.RendererAPI), &sRendererModule))
    {
        THROW(std::format("Failed to load plugin '{}'", Renderer::ApiToModuleString(config.RendererAPI)));
    }

    if (!sRendererModule.LoadSymbol("CreatePlugin") || !sRendererModule.LoadSymbol("DestroyPlugin"))
    {
        THROW(std::format("Failed to load symbols of plugin '{}'.", Renderer::ApiToModuleString(config.RendererAPI)));
    }

    auto CreatePluginFn = sRendererModule.GetSymbol<CreateRendererPluginFn>("CreatePlugin");
    sRendererBackend = CreatePluginFn(config);
}

void Renderer::Shutdown()
{
    auto DestroyPluginFn = sRendererModule.GetSymbol<DestroyRendererPluginFn>("DestroyPlugin");
    DestroyPluginFn(sRendererBackend);
}

auto Renderer::ApiToModuleString(RendererAPI api) -> const char*
{
    switch (api)
    {
        case RendererAPI::Vulkan:       return "VulkanRenderer";
        case RendererAPI::OpenGL:       return "OpenGLRenderer";
        case RendererAPI::Direct3D11:   return "Direct3D11Renderer";
        case RendererAPI::Direct3D12:   return "Direct3D12Renderer";
        case RendererAPI::Metal:        return "MetalRenderer";
    }
}