#include "Renderer.hpp"
#include "Vulkan/VulkanBackend.hpp"

static RendererBackend* sRendererBackend;

void Renderer::Initialize(const ApplicationConfig& config)
{
    SWITCH_RENDERER_API(sRendererBackend = new, Backend(config.WindowWidth, config.WindowHeight));
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