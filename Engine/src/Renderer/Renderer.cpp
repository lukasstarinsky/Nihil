#include "Renderer.hpp"
#include "Vulkan/VulkanBackend.hpp"

static RendererBackend* sRendererBackend;

void Renderer::Initialize(const ApplicationConfig& config)
{
    switch (config.RendererAPI)
    {
        case RendererAPI::Vulkan:
            sRendererBackend = new VulkanBackend(config.WindowWidth, config.WindowHeight);
            break;
        case RendererAPI::OpenGL:
            break;
        case RendererAPI::Direct3D11:
            break;
        case RendererAPI::Direct3D12:
            break;
        case RendererAPI::Metal:
            break;
    }
//    if (!DynamicLibrary::Load(Renderer::ApiToString(config.RendererAPI), &sRendererModule))
//    {
//        NTHROW(std::format("Failed to load '{}' dynamic library.", Renderer::ApiToString(config.RendererAPI)));
//    }
//
//    if (!sRendererModule.LoadFunction("CreatePlugin") || !sRendererModule.LoadFunction("DestroyPlugin"))
//    {
//        NTHROW(std::format("Failed to load functions of module '{}'.", Renderer::ApiToString(config.RendererAPI)));
//    }
//
//    auto CreatePluginFn { sRendererModule.GetFunction<CreateRendererPluginFn>("CreatePlugin") };
//    sRendererPlugin = CreatePluginFn(config.WindowWidth, config.WindowHeight);
}

void Renderer::Shutdown()
{
    delete sRendererBackend;
}