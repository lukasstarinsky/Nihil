#pragma once

#include "Shader.hpp"
#include "Material.hpp"
#include "Buffer.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"

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

    virtual auto GetApi() const -> RendererAPI = 0;
    virtual auto GetApiString() const -> const char* = 0;

    virtual void BeginFrame(f32 r, f32 g, f32 b, f32 a) const = 0;
    virtual void OnResize(i32 width, i32 height) const = 0;
    virtual void EndFrame() const = 0;

    virtual auto CreateShader(const ShaderSpecification& shaderSpec) const -> ShaderPtr = 0;
    virtual auto CreateMaterial(const ShaderPtr& vertexShader, const ShaderPtr& fragmentShader) const -> MaterialPtr = 0;
    virtual auto CreateBuffer(BufferType bufferType, const void* data, i32 size, i32 uniformBinding) const -> BufferPtr = 0;
    virtual auto CreateMesh(const MeshSpecification& meshSpec) const -> MeshPtr = 0;
    virtual auto CreateTexture(const TextureSpecification& textureSpec) const -> TexturePtr = 0;

    virtual void Draw(const MeshPtr& mesh) const = 0;
};