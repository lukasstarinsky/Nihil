#include "Renderer.hpp"
#include "RendererPlugin.hpp"
#include "Platform/Platform.hpp"

static DynamicLibrary sRendererModule;
static RendererPlugin* sRendererPlugin;

void Renderer::Initialize(RendererAPI api)
{
    if (!Platform::LoadDynamicLibrary(Renderer::ApiToString(api), sRendererModule))
    {
        NTHROW(std::format("Failed to load '{}' dynamic library.", Renderer::ApiToString(api)));
    }

    if (!Platform::LoadDynamicLibraryFunction(sRendererModule, "CreatePlugin") ||
        !Platform::LoadDynamicLibraryFunction(sRendererModule, "DestroyPlugin"))
    {
        NTHROW(std::format("Failed to load functions of module '{}'.", Renderer::ApiToString(api)));
    }

    auto CreatePluginFn { reinterpret_cast<CreateRendererPlugin>(sRendererModule.Functions[0]) };
    sRendererPlugin = CreatePluginFn();
}

void Renderer::Shutdown()
{
    auto DestroyPluginFn { reinterpret_cast<DestroyRendererPlugin>(sRendererModule.Functions[1]) };
    DestroyPluginFn(sRendererPlugin);
    Platform::UnloadDynamicLibrary(sRendererModule);
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