#include <queue>
#include "Input.hpp"
#include "Event.hpp"

struct InputState
{
    bool Keys[static_cast<u16>(Key::Total)];
    bool Buttons[static_cast<u16>(Button::Total)];

    bool LastKeys[static_cast<u16>(Key::Total)];
    bool LastButtons[static_cast<u16>(Button::Total)];
};

static InputState sState;

void Input::ProcessKey(Key key, bool isPressed)
{
    ProcessKey(key, isPressed, sState.Keys[static_cast<i32>(key)]);
}

void Input::ProcessKey(Key key, bool isPressed, bool wasPressed)
{
    i32 index = static_cast<i32>(key);

    sState.LastKeys[index] = wasPressed;
    sState.Keys[index] = isPressed;

    if (sState.LastKeys[index] != isPressed)
    {
        KeyEvent e { .Key = key };
        EventDispatcher::Dispatch({ .Type = isPressed ? Event::KeyPress : Event::KeyRelease , .KeyEvent = e });
    }
}

void Input::ProcessButton(Button button, bool isPressed)
{
    i32 index = static_cast<i32>(button);

    sState.LastButtons[index] = sState.Buttons[index];
    sState.Buttons[index] = isPressed;

    if (sState.LastButtons[index] != isPressed)
    {
        MouseEvent e { .Button = button };
        EventDispatcher::Dispatch({ .Type = isPressed ? Event::MousePress : Event::MouseRelease, .MouseEvent = e });
    }
}

auto Input::IsKeyDown(Key key) -> bool
{
    return sState.Keys[static_cast<i32>(key)];
}

auto Input::IsButtonDown(Button button) -> bool
{
    return sState.Buttons[static_cast<i32>(button)];
}