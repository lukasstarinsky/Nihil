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

class NIHIL_API Logger
{
public:
    void SetLogLevel(LogLevel logLevel);

    template<typename... Args>
    void Log(LogLevel severity, std::format_string<Args...> fmt, Args&&... args)
    {
        if (mLogLevel > severity)
            return;

        std::string logMessage { std::format(fmt, std::forward<Args>(args)...) };
        LogImpl(severity, logMessage);
    }

    static Logger& Get();
private:
    void LogImpl(LogLevel severity, std::string_view message);
private:
    LogLevel mLogLevel { LogLevel::Trace };
};

#define LOG_SETLOGLEVEL(level) Logger::Get().SetLogLevel(level)
#define LOG_TRACE(fmt, ...) Logger::Get().Log(LogLevel::Trace, fmt, __VA_ARGS__)
#define LOG_DEBUG(fmt, ...) Logger::Get().Log(LogLevel::Debug, fmt, __VA_ARGS__)
#define LOG_INFO(fmt, ...)  Logger::Get().Log(LogLevel::Info, fmt, __VA_ARGS__)
#define LOG_WARN(fmt, ...)  Logger::Get().Log(LogLevel::Warn, fmt, __VA_ARGS__)
#define LOG_ERROR(fmt, ...) Logger::Get().Log(LogLevel::Error, fmt, __VA_ARGS__)
#define LOG_FATAL(fmt, ...) Logger::Get().Log(LogLevel::Fatal, fmt, __VA_ARGS__)