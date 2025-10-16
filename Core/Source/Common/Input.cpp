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
        EventDispatcher::Dispatch(KeyEvent { .Key = key, .Type = isPressed ? EventType::KeyPress : EventType::KeyRelease });
    }
}

void Input::ProcessButton(Button button, bool isPressed, Vec2f position)
{
    i32 index = static_cast<i32>(button);

    sState.LastButtons[index] = sState.Buttons[index];
    sState.Buttons[index] = isPressed;

    if (sState.LastButtons[index] != isPressed)
    {
        EventDispatcher::Dispatch(MouseEvent { .Button = button, .Position = position, .Type = isPressed ? EventType::MousePress : EventType::MouseRelease });
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