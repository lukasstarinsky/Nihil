#pragma once

#include "Common/Application.hpp"
#include "Common/Event.hpp"
#include "Platform/Platform.hpp"
#include "RendererAPI.hpp"
#include "RendererBackend.hpp"
#include "RenderState.hpp"
#include "Camera.hpp"

struct CameraData
{
    Mat4f Projection;
    Mat4f View;
};

struct ObjectData
{
    Mat4f Model;
};

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

    void Enable(RenderState state);
    void Disable(RenderState state);

    void BeginScene(const Camera& camera);
    void BeginObject(const Mat4f& model);
    void Draw(const MeshPtr& mesh, const Mat4f& model = Mat4f::Identity());
    void Draw(const MeshPtr& mesh, const SubMesh& subMesh, const Mat4f& model = Mat4f::Identity());
    void Draw(const MeshPtr& mesh, const MaterialInstancePtr& materialOverride, const Mat4f& model = Mat4f::Identity());
    void DrawInstanced(const MeshPtr& mesh, i32 instanceCount);

    auto ApiToModuleString(RendererAPI api) -> const char*;
}