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

    auto ApiToModuleString(RendererAPI api) -> const char*;
}

class RendererBackend
{
public:
    virtual ~RendererBackend() = default;

    virtual auto GetType() const -> RendererAPI = 0;
    virtual auto GetTypeString() const -> const char* = 0;
};

using CreateRendererPluginFn = RendererBackend*(*)(const ApplicationConfig& appConfig);
using DestroyRendererPluginFn = void(*)(RendererBackend*);