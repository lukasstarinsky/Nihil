#include "Renderer.hpp"
#include "RendererPlugin.hpp"
#include "Platform/Platform.hpp"

static DynamicLibrary sRendererModule;
static RendererPlugin* sRendererPlugin;

void Renderer::Initialize(i32 width, i32 height, RendererAPI api)
{
    if (!DynamicLibrary::Load(Renderer::ApiToString(api), &sRendererModule))
        NTHROW(std::format("Failed to load '{}' dynamic library.", Renderer::ApiToString(api)));

    if (!sRendererModule.LoadFunction("CreatePlugin") || !sRendererModule.LoadFunction("DestroyPlugin"))
        NTHROW(std::format("Failed to load functions of module '{}'.", Renderer::ApiToString(api)));

    auto CreatePluginFn { sRendererModule.GetFunction<CreateRendererPlugin>("CreatePlugin") };
    sRendererPlugin = CreatePluginFn(width, height);
}

void Renderer::Shutdown()
{
    delete sRendererPlugin;
//    auto DestroyPluginFn { sRendererModule.GetFunction<DestroyRendererPlugin>("DestroyPlugin") };
//    DestroyPluginFn(sRendererPlugin);
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