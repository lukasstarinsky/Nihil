#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

struct PlatformState
{
    HWND WindowHandle;
    HINSTANCE Instance;
};