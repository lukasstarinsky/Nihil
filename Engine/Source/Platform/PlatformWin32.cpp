#include "PlatformWin32.hpp"
#include "Core/Event.hpp"
#include "Platform/Platform.hpp"

static PlatformState sState;
static auto ProcessMessage(HWND handle, u32 msg, WPARAM wParam, LPARAM lParam) -> LRESULT;

void Platform::Initialize(const ApplicationConfig& config)
{
    Logger::Trace("Initializing Win32 Platform...");
    sState.Instance = GetModuleHandle(nullptr);

    WNDCLASSEX wndClass {};
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.lpfnWndProc = ProcessMessage;
    wndClass.hInstance = sState.Instance;
    wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndClass.lpszClassName = "nihil_window";
    Ensure(RegisterClassEx(&wndClass), "Win32: RegisterClassEx() failed with error code: {}", GetLastError());

    DWORD dwStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX | WS_SYSMENU;
    RECT rect { 0, 0, static_cast<i32>(config.WindowWidth), static_cast<i32>(config.WindowHeight) };
    AdjustWindowRect(&rect, dwStyle, false);

    sState.WindowHandle = CreateWindow(
        wndClass.lpszClassName,
        config.Name.c_str(),
        dwStyle,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        nullptr,
        nullptr,
        sState.Instance,
        nullptr
    );
    Ensure(sState.WindowHandle, "Win32: CreateWindow() failed with error code: {}", GetLastError());
    sState.DeviceContext = GetDC(sState.WindowHandle);

    QueryPerformanceFrequency(&sState.Frequency);

    ShowWindow(sState.WindowHandle, SW_SHOW);
}

void Platform::Shutdown()
{
    Logger::Trace("Shutting down Win32 Platform...");
    ReleaseDC(sState.WindowHandle, sState.DeviceContext);
    DestroyWindow(sState.WindowHandle);
    UnregisterClass("nihil_window", sState.Instance);
}

void Platform::PollEvents()
{
    MSG msg;
    while (PeekMessage(&msg, sState.WindowHandle, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

auto Platform::GetTimeSeconds() -> f64
{
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    return static_cast<f64>(time.QuadPart) / static_cast<f64>(sState.Frequency.QuadPart);
}

auto ProcessMessage(HWND handle, u32 msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (msg)
    {
        case WM_CLOSE:
        {
            EventDispatcher::Dispatch({ .Type = Event::ApplicationQuit, .ApplicationEvent = { } });
            break;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
        case WM_SIZE:
        {
            RECT rect {};
            GetClientRect(sState.WindowHandle, &rect);

            ApplicationEvent e { .Width = rect.right - rect.left, .Height = rect.bottom - rect.top };
            EventDispatcher::Dispatch({ .Type = Event::ApplicationResize, .ApplicationEvent = e });
            break;
        }
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            BOOL wasKeyDown = (lParam & (1 << 30)) != 0;

            bool isPressed = msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN;
            Input::ProcessKey(static_cast<Key>(wParam), isPressed, wasKeyDown);

            break;
        }
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        {
            bool isPressed = msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN;

            Button button = Button::Middle;
            switch (msg)
            {
                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP:
                    button = Button::Left;
                    break;
                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP:
                    button = Button::Right;
                    break;
                case WM_MBUTTONDOWN:
                case WM_MBUTTONUP:
                    button = Button::Middle;
                    break;
            }

            Input::ProcessButton(button, isPressed);
            break;
        }
    }

    return DefWindowProc(handle, msg, wParam, lParam);
}

auto Platform::GetState() -> const PlatformState&
{
    return sState;
}

auto DynamicLibrary::Load(const char* name, DynamicLibrary* outDynamicLibrary) -> bool
{
    char buffer[256] {};
    sprintf_s(buffer, "%s.dll", name);

    HMODULE module = LoadLibrary(buffer);
    if (!module)
        return false;

    outDynamicLibrary->mName = buffer;
    outDynamicLibrary->mHandle = module;
    return true;
}

auto DynamicLibrary::Unload() const -> bool
{
    ASSERT(mHandle);

    return FreeLibrary(static_cast<HMODULE>(mHandle));
}

auto DynamicLibrary::LoadSymbol(const char* name) -> bool
{
    ASSERT(mHandle);

    FARPROC symbolPtr = GetProcAddress(static_cast<HMODULE>(mHandle), name);
    if (!symbolPtr)
        return false;

    mSymbols[name] = reinterpret_cast<void*>(symbolPtr);
    return true;
}

void Console::Print(std::string_view message, LogLevel severity)
{
    auto consoleHandle = GetStdHandle((severity == LogLevel::Error || severity == LogLevel::Fatal) ? STD_ERROR_HANDLE : STD_OUTPUT_HANDLE);
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