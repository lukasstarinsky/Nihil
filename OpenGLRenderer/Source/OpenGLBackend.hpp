#pragma once

#include "Core/Application.hpp"
#include "Renderer/Renderer.hpp"

class OpenGLBackend : public RendererBackend
{
public:
    explicit OpenGLBackend(const ApplicationConfig& config);
    ~OpenGLBackend() override;

    auto GetType() const -> RendererAPI override;
    auto GetTypeString() const -> const char* override;

    void BeginFrame(f32 r, f32 g, f32 b, f32 a) const override;
    void EndFrame() const override;
};