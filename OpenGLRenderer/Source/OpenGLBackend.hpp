#pragma once

#include "Core/Application.hpp"
#include "OpenGLCommon.hpp"

class OpenGLBackend : public RendererBackend
{
public:
    explicit OpenGLBackend(const ApplicationConfig& appConfig);
    ~OpenGLBackend() override;

    auto GetApi() const -> RendererAPI override;

    void BeginFrame(f32 r, f32 g, f32 b, f32 a) const override;
    void OnResize(i32 width, i32 height) const override;
    void EndFrame() const override;

    auto CreateShader(const ShaderSpecification& shaderSpec) const -> ShaderPtr override;
    auto CreateMaterial(const ShaderPtr& vertexShader, const ShaderPtr& fragmentShader) const -> MaterialPtr override;
    auto CreateBuffer(BufferType bufferType, const void* data, i32 size, i32 uniformBinding) const -> BufferPtr override;
    auto CreateMesh(const MeshSpecification& meshSpec) const -> MeshPtr override;
    auto CreateTexture(const TextureSpecification& textureSpec) const -> TexturePtr override;

    void Draw(const MeshPtr& mesh) const override;
};