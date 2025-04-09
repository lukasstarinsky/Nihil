#pragma once

#include "Defines.hpp"

enum class RendererAPI;

struct ApplicationConfig
{
    u32 WindowWidth = 1024;
    u32 WindowHeight = 768;
    std::string Name = "Nihil Application";
    RendererAPI RendererAPI {};
};

struct ApplicationState
{
    bool IsRunning = true;
    bool IsSuspended;
};

///
/// Note: Use OnInitialize to create resources, use constructor only to initialize ApplicationConfig
///
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