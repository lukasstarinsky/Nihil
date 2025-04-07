#pragma once

#include "Core/Application.hpp"
#include "OpenGLCommon.hpp"

class OpenGLBackend : public RendererBackend
{
public:
    explicit OpenGLBackend(const ApplicationConfig& config);
    ~OpenGLBackend() override;

    auto GetType() const -> RendererAPI override;
    auto GetTypeString() const -> const char* override;

    void BeginFrame(f32 r, f32 g, f32 b, f32 a) const override;
    void EndFrame() const override;

    auto CreateShader(const std::string& filePath, ShaderType shaderType) const -> ShaderPtr override;
    auto CreateMaterial(const ShaderPtr& vertexShader, const ShaderPtr& fragmentShader) const -> MaterialPtr override;
};