#pragma once

#include "Core/Application.hpp"
#include "Core/Logger.hpp"

namespace Platform
{
    bool Initialize(const ApplicationConfig& config);
    void Shutdown();
}

namespace Console
{
    void Print(std::string_view message, LogLevel severity);
}