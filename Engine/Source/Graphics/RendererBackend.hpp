#pragma once

#include "RendererAPI.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "Buffer.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"

class RendererBackend
{
public:
    virtual ~RendererBackend() = default;

    virtual auto GetApi() const -> RendererAPI = 0;
    virtual auto GetApiString() const -> const char* = 0;

    virtual void BeginFrame(f32 r, f32 g, f32 b, f32 a) const = 0;
    virtual void OnResize(i32 width, i32 height) const = 0;
    virtual void EndFrame() const = 0;

    virtual auto CreateShader(const ShaderCreateInfo& createInfo) const -> ShaderPtr = 0;
    virtual auto CreateMaterial(const MaterialCreateInfo& createInfo) const -> MaterialPtr = 0;
    virtual auto CreateBuffer(const BufferCreateInfo& createInfo) const -> BufferPtr = 0;
    virtual auto CreateMesh(const MeshCreateInfo& createInfo) const -> MeshPtr = 0;
    virtual auto CreateTexture(const TextureCreateInfo& CreateInfo) const -> TexturePtr = 0;

    virtual void Draw(const SubMesh& subMesh) const = 0;
};