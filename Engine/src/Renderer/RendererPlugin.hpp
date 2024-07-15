#pragma once

class RendererPlugin
{
public:
    virtual ~RendererPlugin() = default;
};

using CreateRendererPlugin = RendererPlugin* (*)();
using DestroyRendererPlugin = void (*)(RendererPlugin*);