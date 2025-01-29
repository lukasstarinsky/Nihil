#pragma once

#include "Core/Application.hpp"

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
    void Initialize(const ApplicationConfig& config);
    void Shutdown();

    auto ApiToString(RendererAPI api) -> const char*;
}

class RendererBackend
{
public:
    virtual ~RendererBackend() = default;
};