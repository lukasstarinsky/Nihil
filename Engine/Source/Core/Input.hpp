#pragma once

#include <optional>
#include "Math/Math.hpp"

enum class Button
{
    Left,
    Right,
    Middle,
    Total
};

enum class Key
{
    Invalid     = 0,
    BackSpace   = 0x08,
    Enter       = 0x0D,
    Tab         = 0x09,
    Shift       = 0x10,
    Control     = 0x11,
    Alt         = 0x12,
    CapsLock    = 0x14,
    Escape      = 0x1B,
    Space       = 0x20,
    PageUp      = 0x21,
    PageDown    = 0x22,
    End         = 0x23,
    Home        = 0x24,
    LeftArrow   = 0x25,
    UpArrow     = 0x26,
    RightArrow  = 0x27,
    DownArrow   = 0x28,
    PrntScr     = 0x2C,
    Insert      = 0x2D,
    Delete      = 0x2E,
    Num0        = 0x30,
    Num1        = 0x31,
    Num2        = 0x32,
    Num3        = 0x33,
    Num4        = 0x34,
    Num5        = 0x35,
    Num6        = 0x36,
    Num7        = 0x37,
    Num8        = 0x38,
    Num9        = 0x39,
    A           = 0x41,
    B           = 0x42,
    C           = 0x43,
    D           = 0x44,
    E           = 0x45,
    F           = 0x46,
    G           = 0x47,
    H           = 0x48,
    I           = 0x49,
    J           = 0x4A,
    K           = 0x4B,
    L           = 0x4C,
    M           = 0x4D,
    N           = 0x4E,
    O           = 0x4F,
    P           = 0x50,
    Q           = 0x51,
    R           = 0x52,
    S           = 0x53,
    T           = 0x54,
    U           = 0x55,
    V           = 0x56,
    W           = 0x57,
    X           = 0x58,
    Y           = 0x59,
    Z           = 0x5A,
    NumPad0     = 0x60,
    NumPad1     = 0x61,
    NumPad2     = 0x62,
    NumPad3     = 0x63,
    NumPad4     = 0x64,
    NumPad5     = 0x65,
    NumPad6     = 0x66,
    NumPad7     = 0x67,
    NumPad8     = 0x68,
    NumPad9     = 0x69,
    F1          = 0x70,
    F2          = 0x71,
    F3          = 0x72,
    F4          = 0x73,
    F5          = 0x74,
    F6          = 0x75,
    F7          = 0x76,
    F8          = 0x77,
    F9          = 0x78,
    F10         = 0x79,
    F11         = 0x7A,
    F12         = 0x7B,
    NumLock     = 0x90,
    ScrollLock  = 0x91,
    Total
};

namespace Input
{
    void ProcessKey(Key key, bool isPressed);
    void ProcessKey(Key key, bool isPressed, bool wasPressed);
    void ProcessButton(Button button, bool isPressed);

    auto NIHIL_API IsKeyDown(Key key) -> bool;
    auto NIHIL_API IsButtonDown(Button button) -> bool;
}