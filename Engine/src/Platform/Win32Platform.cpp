#include <Windows.h>
#include "Platform.hpp"

struct PlatformState
{
    HWND WindowHandle;
    HINSTANCE Instance;
    bool Initialized;
};

static PlatformState sState;

bool Platform::Initialize(const ApplicationConfig& config)
{
    sState.Instance = GetModuleHandle(nullptr);

    WNDCLASSEX wndClass {};
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.lpfnWndProc = DefWindowProc;
    wndClass.hInstance = sState.Instance;
    wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndClass.lpszClassName = "nihil_window";
    if (!RegisterClassEx(&wndClass))
    {
        LOG_FATAL("Win32: RegisterClassEx() failed with error code: %d.", GetLastError());
        return false;
    }

    DWORD dwStyle { WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX | WS_SYSMENU };
    RECT rect { 0, 0, config.WindowWidth, config.WindowHeight };
    AdjustWindowRect(&rect, dwStyle, false);

    sState.WindowHandle = CreateWindow(
        wndClass.lpszClassName,
        config.WindowTitle,
        dwStyle,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        nullptr,
        nullptr,
        sState.Instance,
        nullptr
    );
    if (!sState.WindowHandle)
    {
        LOG_FATAL("Win32: CreateWindow() failed with error code: %d.", GetLastError());
        return false;
    }

    ShowWindow(sState.WindowHandle, SW_SHOW);
    sState.Initialized = true;
    return true;
}

void Platform::Shutdown()
{
    if (!sState.Initialized)
        return;

    DestroyWindow(sState.WindowHandle);
    UnregisterClass("nihil_window", sState.Instance);
}

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