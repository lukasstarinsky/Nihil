#pragma once

#include "Core/Application.hpp"

enum class RendererAPI
{
    Vulkan = 0,
    OpenGL,
    Direct3D11,
    Direct3D12,
    Metal
};

namespace Renderer
{
    void Initialize(const ApplicationConfig& config);
    void Shutdown();
}

class RendererBackend
{
public:
    virtual ~RendererBackend() = default;
};

#define SWITCH_RENDERER_API(exprStart, exprEnd) \
do {                                    \
    switch (config.RendererAPI)         \
    {                                   \
        case RendererAPI::Vulkan:       \
            exprStart Vulkan##exprEnd;  \
            break;                      \
        case RendererAPI::OpenGL:       \
        case RendererAPI::Direct3D11:   \
        case RendererAPI::Direct3D12:   \
        case RendererAPI::Metal:        \
            break;                      \
    }                                   \
} while(0)