#pragma once

class RendererPlugin
{
public:
    virtual ~RendererPlugin() = default;

    virtual void Initialize() = 0;
};

using CreateRendererPlugin = RendererPlugin* (*)();