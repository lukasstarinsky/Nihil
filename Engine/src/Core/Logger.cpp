#include "Platform/Platform.hpp"
#include "Logger.hpp"

void Logger::SetLogLevel(LogLevel logLevel)
{
    mLogLevel = logLevel;
}

void Logger::LogImpl(LogLevel severity, std::string_view message)
{
    const char* severityStr[] { "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL" };

    std::string logMessage { std::format("{} [{}]: {}\n", Utilities::GetFormattedTime(true), severityStr[static_cast<i32>(severity)], message) };
    Console::Print(logMessage, severity);
}

Logger& Logger::Get()
{
    static Logger instance;
    return instance;
}