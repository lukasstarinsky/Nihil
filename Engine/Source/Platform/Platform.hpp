#pragma once

#include "Core/Application.hpp"
#include "DynamicLibrary.hpp"
#include "Console.hpp"

struct PlatformState;

namespace Platform
{
    void Initialize(const ApplicationConfig& config);
    void Shutdown();
    void PollEvents();
    auto GetTimeSeconds() -> f64;

    NIHIL_API auto GetState() -> const PlatformState&;
}