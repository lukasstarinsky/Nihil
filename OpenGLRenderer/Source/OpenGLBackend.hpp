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
private:
    void ResolveGLFunctions() const;
};