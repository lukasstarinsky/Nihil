#include "Platform/Platform.hpp"
#include "Logger.hpp"

static LogLevel sLogLevel { LogLevel::Trace };

LogLevel Logger::GetLogLevel()
{
    return sLogLevel;
}

void Logger::SetLogLevel(LogLevel logLevel)
{
    sLogLevel = logLevel;
}

void Logger::Log(LogLevel severity, std::string_view message)
{
    if (sLogLevel > severity)
        return;

    const char* severityStr[] { "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL" };

    std::string logMessage { std::format("{} [{}]: {}\n", Utilities::GetFormattedTime(true), severityStr[static_cast<i32>(severity)], message) };
    Console::Print(logMessage, severity);
}