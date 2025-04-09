#pragma once

#include "Core/Application.hpp"
#include "OpenGLCommon.hpp"

class OpenGLBackend : public RendererBackend
{
public:
    explicit OpenGLBackend(const ApplicationConfig& appConfig);
    ~OpenGLBackend() override;

    auto GetType() const -> RendererAPI override;
    auto GetTypeString() const -> const char* override;

    void BeginFrame(f32 r, f32 g, f32 b, f32 a) const override;
    void OnResize(i32 width, i32 height) const override;
    void EndFrame() const override;

    auto CreateShader(const std::string& filePath, ShaderType shaderType) const -> ShaderPtr override;
    auto CreateMaterial(const ShaderPtr& vertexShader, const ShaderPtr& fragmentShader) const -> MaterialPtr override;
    auto CreateBuffer(BufferType bufferType, const void* data, i32 size, i32 uniformBinding) const -> BufferPtr override;
    auto CreateMesh(std::span<const Vertex> vertices, std::span<const Index> indices) const -> MeshPtr override;

    void Draw(const MeshPtr& mesh) const override;
};