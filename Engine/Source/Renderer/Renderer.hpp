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

class RendererBackend
{
public:
    virtual ~RendererBackend() = default;

    virtual auto GetType() const -> RendererAPI = 0;
    virtual auto GetTypeString() const -> const char* = 0;
};

namespace Renderer
{
    void Initialize(const ApplicationConfig& config);
    void Shutdown();

    auto ApiToModuleString(RendererAPI api) -> const char*;

    using CreatePluginFn = RendererBackend*(*)(const ApplicationConfig& appConfig, std::exception_ptr& exceptionPtr);
    using DestroyPluginFn = void(*)(RendererBackend*);
}