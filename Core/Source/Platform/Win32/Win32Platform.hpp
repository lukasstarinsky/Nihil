#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <hidusage.h>

struct PlatformState
{
    HWND WindowHandle;
    HINSTANCE Instance;
    HDC DeviceContext;
    LARGE_INTEGER Frequency;
};