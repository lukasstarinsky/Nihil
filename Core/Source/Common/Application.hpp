#pragma once

#include <chrono>

#include "Defines.hpp"

enum class RendererAPI : u32;

struct ApplicationConfig
{
    i32 WindowWidth = 1024;
    i32 WindowHeight = 768;
    std::string Name = "Nihil Application";
    RendererAPI RendererAPI {};
};

struct ApplicationState
{
    bool IsRunning = true;
    bool IsSuspended;
    std::chrono::steady_clock::time_point LastFrameTime {};
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
    virtual void OnUpdate(f32 deltaTimeSeconds) = 0;
    virtual void OnResize() = 0;
    virtual void OnRender() = 0;
    virtual void OnUIRender() = 0;
    virtual void OnShutdown() = 0;
public:
    ApplicationConfig Config {};
    ApplicationState State {};
};