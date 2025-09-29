#pragma once

#include "Common/Application.hpp"
#include "DynamicLibrary.hpp"
#include "MappedFile.hpp"

struct PlatformState;
enum class LogLevel;

class NIHIL_API Platform
{
public:
    static void Initialize(const ApplicationConfig& config);
    static void Shutdown();
    static void PollEvents();
    static void Print(std::string_view message, LogLevel severity);

    static auto GetTimeSeconds() -> f64;
    static auto GetState() -> const PlatformState&;
};