#pragma once

#include "Defines.hpp"

enum class RendererAPI;

struct ApplicationConfig
{
    i32 WindowWidth = 1024;
    i32 WindowHeight = 768;
    const char* WindowTitle = "Nihil Application";
    RendererAPI RendererAPI {};
};

struct ApplicationState
{
    bool IsRunning = true;
    bool IsSuspended;
};

class NIHIL_API Application
{
public:
    Application() = default;

    virtual ~Application() = default;
    virtual void OnInitialize() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnRender() = 0;
    virtual void OnShutdown() = 0;
public:
    ApplicationConfig Config {};
    ApplicationState State {};
};