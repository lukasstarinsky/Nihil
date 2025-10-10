#pragma once

#include "Common/Application.hpp"
#include "OpenGLCommon.hpp"

class OpenGLBackend : public RendererBackend
{
public:
    explicit OpenGLBackend(const ApplicationConfig& appConfig, const PlatformState& platformState);
    ~OpenGLBackend() override;

    auto GetApi() const -> RendererAPI override;
    auto GetApiString() const -> const char* override;

    void BeginFrame(f32 r, f32 g, f32 b, f32 a) const override;
    void OnResize(i32 width, i32 height) const override;
    void EndFrame() const override;

    auto CreateShader(const ShaderCreateInfo& shaderCreateInfo) const -> ShaderPtr override;
    auto CreateMaterial(const MaterialCreateInfo& materialCreateInfo) const -> MaterialPtr override;
    auto CreateBuffer(const BufferCreateInfo& bufferCreateInfo) const -> BufferPtr override;
    auto CreateMesh(const MeshCreateInfo& meshCreateInfo) const -> MeshPtr override;
    auto CreateTexture(const TextureCreateInfo& textureCreateInfo) const -> TexturePtr override;

    void Draw(const MeshPtr& mesh, u32 subMeshIndex) const override;
private:
    const PlatformState& mPlatformState;
};