#pragma once

#include "Core/Defines.hpp"

class RendererPlugin
{
public:
    virtual ~RendererPlugin() = default;
};

using CreateRendererPlugin = RendererPlugin* (*)(i32 width, i32 height);
using DestroyRendererPlugin = void (*)(RendererPlugin*);