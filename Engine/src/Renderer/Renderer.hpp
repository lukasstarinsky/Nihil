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
    void Initialize(i32 width, i32 height, RendererAPI api);
    void Shutdown();

    const char* ApiToString(RendererAPI api);
}