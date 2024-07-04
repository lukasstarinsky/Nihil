#include <Windows.h>
#include "Platform.hpp"

#pragma region Console
void Console::Print(std::string_view message, LogLevel severity)
{
    HANDLE consoleHandle { GetStdHandle((severity == LogLevel::Error || severity == LogLevel::Fatal) ? STD_ERROR_HANDLE : STD_OUTPUT_HANDLE) };
    CONSOLE_SCREEN_BUFFER_INFO consoleSbi {};
    GetConsoleScreenBufferInfo(consoleHandle, &consoleSbi);

    u16 color[] {
        FOREGROUND_INTENSITY,
        FOREGROUND_INTENSITY,
        FOREGROUND_GREEN | FOREGROUND_BLUE,
        FOREGROUND_RED | FOREGROUND_GREEN,
        FOREGROUND_RED | FOREGROUND_INTENSITY,
        FOREGROUND_RED | FOREGROUND_INTENSITY
    };

    SetConsoleTextAttribute(consoleHandle, color[static_cast<i32>(severity)]);
    WriteConsole(consoleHandle, message.data(), message.size(), nullptr, nullptr);
    SetConsoleTextAttribute(consoleHandle, consoleSbi.wAttributes);
}
#pragma endregion