#pragma once

enum class RendererAPI
{
    Vulkan = 0,
    OpenGL,
    Direct3D11,
    Direct3D12,
    Metal
};

namespace Renderer
{
    void Initialize(RendererAPI api);
    void Shutdown();

    static const char* ApiToString(RendererAPI api);
}