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
    auto GetState() -> PlatformState*;
}