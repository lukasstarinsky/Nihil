#pragma once

#include "Core/Defines.hpp"

// TODO: Switch to compile time linking?
class RendererPlugin
{
public:
    virtual ~RendererPlugin() = default;
};

using CreateRendererPluginFn = RendererPlugin* (*)(i32 width, i32 height);
using DestroyRendererPluginFn = void (*)(RendererPlugin*);