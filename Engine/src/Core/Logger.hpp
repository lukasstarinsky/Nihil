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
    NIHIL_API auto GetLogLevel() -> LogLevel;
    NIHIL_API void SetLogLevel(LogLevel logLevel);
    NIHIL_API void Log(LogLevel severity, std::string_view message);

    template<typename... Args>
    void Log(LogLevel severity, std::format_string<Args...> fmt, Args&&... args)
    {
        std::string logMessage { std::format(fmt, std::forward<Args>(args)...) };
        Log(severity, logMessage);
    }
}

#define LOG_TRACE(fmt, ...)     Logger::Log(LogLevel::Trace, fmt, __VA_ARGS__)
#define LOG_DEBUG(fmt, ...)     Logger::Log(LogLevel::Debug, fmt, __VA_ARGS__)
#define LOG_INFO(fmt, ...)      Logger::Log(LogLevel::Info, fmt, __VA_ARGS__)
#define LOG_WARN(fmt, ...)      Logger::Log(LogLevel::Warn, fmt, __VA_ARGS__)
#define LOG_ERROR(fmt, ...)     Logger::Log(LogLevel::Error, fmt, __VA_ARGS__)
#define LOG_FATAL(fmt, ...)     Logger::Log(LogLevel::Fatal, fmt, __VA_ARGS__)