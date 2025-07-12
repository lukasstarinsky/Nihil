#pragma once

#include "Defines.hpp"
#include "AssetManager.hpp"

enum class RendererAPI;

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
    f64 LastFrameTime;
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
    virtual void OnShutdown() = 0;
public:
    ApplicationConfig Config {};
    ApplicationState State {};
protected:
    std::unique_ptr<AssetManager> mAssetManager {};
};