#pragma once

#include "Common/Application.hpp"
#include "Common/Event.hpp"
#include "Platform/Platform.hpp"
#include "RendererAPI.hpp"
#include "RendererBackend.hpp"
#include "Camera.hpp"

namespace Renderer
{
    using CreatePluginFn = RendererBackend*(*)(const ApplicationConfig& appConfig, const PlatformState& platformState, std::exception_ptr& exceptionPtr);
    using DestroyPluginFn = void(*)(RendererBackend*);

    void Initialize(const ApplicationConfig& config);
    auto OnAppEvent(const ApplicationEvent& e) -> bool;
    void Shutdown();

    auto GetApi() -> RendererAPI;
    auto GetApiString() -> const char*;

    void BeginFrame(f32 r, f32 g, f32 b, f32 a);
    void EndFrame();

    void BeginScene(const Camera& camera);
    void Draw(const MeshPtr& mesh);

    auto ApiToModuleString(RendererAPI api) -> const char*;
}