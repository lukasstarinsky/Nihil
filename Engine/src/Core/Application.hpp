#pragma once

#include "Defines.hpp"

struct ApplicationConfig
{
    i32 WindowWidth { 1024 };
    i32 WindowHeight { 768 };
    const char* WindowTitle { "Nihil Application" };
};

struct ApplicationState
{
    bool IsRunning { true };
    bool IsSuspended;
};

class NIHIL_API Application
{
public:
    Application() = default;
    explicit Application(const ApplicationConfig& config)
        : Config{config} {}

    virtual ~Application() = default;
    virtual void OnInitialize() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnRender() = 0;
public:
    ApplicationConfig Config {};
    ApplicationState State {};
};