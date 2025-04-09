#pragma once

#include "Core/Application.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "Buffer.hpp"
#include "Mesh.hpp"

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

    virtual auto CreateShader(const std::string& filePath, ShaderType shaderType) const -> ShaderPtr = 0;
    virtual auto CreateMaterial(const ShaderPtr& vertexShader, const ShaderPtr& fragmentShader) const -> MaterialPtr = 0;
    virtual auto CreateBuffer(BufferType bufferType, const void* data, i32 size, i32 uniformBinding) const -> BufferPtr = 0;
    virtual auto CreateMesh() const -> MeshPtr = 0;

    virtual void Draw(const MeshPtr& mesh) const = 0;
};

namespace Renderer
{
    using CreatePluginFn = RendererBackend*(*)(const ApplicationConfig& appConfig, std::exception_ptr& exceptionPtr);
    using DestroyPluginFn = void(*)(RendererBackend*);

    void Initialize(const ApplicationConfig& config);
    void Shutdown();

    void BeginFrame(f32 r, f32 g, f32 b, f32 a);
    void EndFrame();

    void NIHIL_API Draw(const MeshPtr& mesh);

    auto ApiToModuleString(RendererAPI api) -> const char*;
}