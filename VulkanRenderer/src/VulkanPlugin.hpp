#pragma once

#include <Nihil.hpp>
#include <Renderer/RendererPlugin.hpp>

class VulkanPlugin : public RendererPlugin
{
public:
    ~VulkanPlugin() override;

    void Initialize() override;
};

extern "C"
{
    NIHIL_API auto CreatePlugin() -> RendererPlugin*;
    NIHIL_API void DestroyPlugin(RendererPlugin* plugin);
}