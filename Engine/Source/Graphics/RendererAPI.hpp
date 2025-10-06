#pragma once

#include "Common/Defines.hpp"

enum class RendererAPI : u32
{
    OpenGL = 0,
    Vulkan,
    Direct3D11,
    Direct3D12,
    Metal
};