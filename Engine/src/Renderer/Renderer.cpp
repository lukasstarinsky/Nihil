#include "Renderer.hpp"
#include "RendererPlugin.hpp"
#include "Platform/Platform.hpp"

static DynamicLibrary sRendererModule;
static RendererPlugin* sRendererPlugin;

bool Renderer::Initialize(RendererAPI api)
{
    if (!Platform::LoadDynamicLibrary(Renderer::ApiToString(api), &sRendererModule))
    {
        LOG_FATAL("Failed to load '%s' dynamic library", Renderer::ApiToString(api));
        return false;
    }

    if (!Platform::LoadDynamicLibraryFunction(sRendererModule, "CreatePlugin") ||
        !Platform::LoadDynamicLibraryFunction(sRendererModule, "DestroyPlugin"))
    {
        LOG_FATAL("Failed to load functions of module '%s'", Renderer::ApiToString(api));
        return false;
    }

    auto CreatePlugin { reinterpret_cast<CreateRendererPlugin>(sRendererModule.Functions[0]) };
    sRendererPlugin = CreatePlugin();
    sRendererPlugin->Initialize();
    return true;
}

void Renderer::Shutdown()
{
    if (sRendererModule.Handle)
    {
        auto DestroyPlugin { reinterpret_cast<void(*)(RendererPlugin*)>(sRendererModule.Functions[1]) };
        DestroyPlugin(sRendererPlugin);

        Platform::UnloadDynamicLibrary(sRendererModule);
    }
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