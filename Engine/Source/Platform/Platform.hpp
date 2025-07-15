#pragma once

#include "Core/Application.hpp"
#include "DynamicLibrary.hpp"
#include "MappedFile.hpp"

struct PlatformState;
enum class LogLevel;

namespace Platform
{
    void Initialize(const ApplicationConfig& config);
    void Shutdown();
    void PollEvents();
    void Print(std::string_view message, LogLevel severity);

    auto GetTimeSeconds() -> f64;
    auto GetState() -> const PlatformState&;
}