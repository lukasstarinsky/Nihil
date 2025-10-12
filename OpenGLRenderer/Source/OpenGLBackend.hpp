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

    auto CreateShader(const ShaderCreateInfo& createInfo) const -> ShaderPtr override;
    auto CreateMaterial(const MaterialCreateInfo& createInfo) const -> MaterialPtr override;
    auto CreateBuffer(const BufferCreateInfo& createInfo) const -> BufferPtr override;
    auto CreateMesh(const MeshCreateInfo& createInfo) const -> MeshPtr override;
    auto CreateTexture(const TextureCreateInfo& createInfo) const -> TexturePtr override;

    void Draw(const SubMesh& subMesh) const override;
private:
    const PlatformState& mPlatformState;
};