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

    auto CreateShader(std::string_view filePath, ShaderType shaderType) const -> std::shared_ptr<Shader> override;
};