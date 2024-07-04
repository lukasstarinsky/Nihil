#include <cstdarg>
#include "Platform/Platform.hpp"
#include "Logger.hpp"

#define CHAR_LIMIT 16'384

#ifdef NDEBUG
static LogLevel sLogLevel { LogLevel::Info };
#else
static LogLevel sLogLevel { LogLevel::Trace };
#endif

void Logger::SetLogLevel(LogLevel logLevel)
{
    sLogLevel = logLevel;
}

void Logger::Log(LogLevel severity, std::string_view message, ...)
{
    if (sLogLevel > severity)
        return;

    const char* severityStr[] { "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL" };

    char logMessage[CHAR_LIMIT] { 0 };
    char logMessageCpy[CHAR_LIMIT] { 0 };

    va_list args;
    va_start(args, message);
    vsnprintf(logMessage, CHAR_LIMIT, message.data(), args);
    va_end(args);

    memcpy_s(logMessageCpy, CHAR_LIMIT, logMessage, CHAR_LIMIT);
    sprintf_s(logMessage, "[%s]: %s\n", severityStr[static_cast<i32>(severity)], logMessageCpy);

    Console::Print(logMessage, severity);
}