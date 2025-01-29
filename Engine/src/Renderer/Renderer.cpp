#include "Renderer.hpp"
#include "Vulkan/VulkanBackend.hpp"

static RendererBackend* sRendererBackend;

void Renderer::Initialize(const ApplicationConfig& config)
{
    switch (config.RendererAPI)
    {
        case RendererAPI::Vulkan:
            sRendererBackend = new VulkanBackend(config);
            break;
        case RendererAPI::OpenGL:
        case RendererAPI::Direct3D11:
        case RendererAPI::Direct3D12:
        case RendererAPI::Metal:
            THROW(std::format("Renderer API '{}' not supported", ApiToString(config.RendererAPI)));
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

auto Renderer::ApiToString(RendererAPI api) -> const char*
{
    switch (api)
    {
        case RendererAPI::Vulkan:       return "Vulkan";
        case RendererAPI::OpenGL:       return "OpenGL";
        case RendererAPI::Direct3D11:   return "Direct3D11";
        case RendererAPI::Direct3D12:   return "Direct3D12";
        case RendererAPI::Metal:        return "Metal";
    }
}