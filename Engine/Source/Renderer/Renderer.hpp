#pragma once

#include "Core/Application.hpp"

enum class RendererAPI
{
    OpenGL = 0,
    Vulkan,
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

    virtual void BeginFrame(f32 r, f32 g, f32 b, f32 a) const = 0;
    virtual void EndFrame() const = 0;
};

namespace Renderer
{
    using CreatePluginFn = RendererBackend*(*)(const ApplicationConfig& appConfig, std::exception_ptr& exceptionPtr);
    using DestroyPluginFn = void(*)(RendererBackend*);

    void Initialize(const ApplicationConfig& config);
    void Shutdown();

    void BeginFrame(f32 r, f32 g, f32 b, f32 a);
    void EndFrame();

    auto ApiToModuleString(RendererAPI api) -> const char*;
}