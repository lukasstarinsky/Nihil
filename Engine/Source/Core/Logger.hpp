#pragma once

#include <format>
#include "Defines.hpp"

enum class LogLevel
{
    Trace = 0,
    Debug,
    Info,
    Warn,
    Error,
    Fatal
};

namespace Logger
{
    NIHIL_API void Log(LogLevel severity, std::string_view message);

    template<typename... Args>
    void Trace(std::format_string<Args...> fmt, Args&&... args)
    {
        Log(LogLevel::Trace, std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void Debug(std::format_string<Args...> fmt, Args&&... args)
    {
        Log(LogLevel::Debug, std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void Info(std::format_string<Args...> fmt, Args&&... args)
    {
        Log(LogLevel::Info, std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void Warn(std::format_string<Args...> fmt, Args&&... args)
    {
        Log(LogLevel::Warn, std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void Error(std::format_string<Args...> fmt, Args&&... args)
    {
        Log(LogLevel::Error, std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void Fatal(std::format_string<Args...> fmt, Args&&... args)
    {
        Log(LogLevel::Fatal, std::format(fmt, std::forward<Args>(args)...));
    }
}