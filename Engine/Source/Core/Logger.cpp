#include "Platform/Platform.hpp"
#include "Logger.hpp"

void Logger::Log(LogLevel severity, std::string_view message)
{
    if (LogLevel::Trace > severity)
        return;

    const char* severityStr[] { "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL" };

    auto logMessage = std::format("{} [{}]: {}\n", Time::GetFormattedTime(true), severityStr[static_cast<i32>(severity)], message);
    Console::Print(logMessage, severity);
}