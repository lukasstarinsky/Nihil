#include <queue>
#include "Input.hpp"
#include "Event.hpp"

struct InputState
{
    bool Keys[static_cast<u16>(Key::Total)];
    bool Buttons[static_cast<u16>(Button::Total)];

    bool LastKeys[static_cast<u16>(Key::Total)];
    bool LastButtons[static_cast<u16>(Button::Total)];

    Vec2i MousePos;
    std::queue<Vec2i> RawDeltaBuffer;
};

static InputState sState;

void Input::ProcessKey(Key key, bool isPressed)
{
    ProcessKey(key, isPressed, sState.Keys[static_cast<i32>(key)]);
}

void Input::ProcessKey(Key key, bool isPressed, bool wasPressed)
{
    i32 index { static_cast<i32>(key) };

    sState.LastKeys[index] = wasPressed;
    sState.Keys[index] = isPressed;

    if (sState.LastKeys[index] != isPressed)
    {
        KeyEvent e { isPressed ? KeyEventType::Press : KeyEventType::Release, key };
        EventDispatcher::Dispatch(EventCategory::Key, &e);
    }
}

void Input::ProcessButton(Button button, bool isPressed)
{
    i32 index { static_cast<i32>(button) };

    sState.LastButtons[index] = sState.Buttons[index];
    sState.Buttons[index] = isPressed;

    if (sState.LastButtons[index] != isPressed)
    {
        MouseEvent e { isPressed ? MouseEventType::Press : MouseEventType::Release, button, {} };
        EventDispatcher::Dispatch(EventCategory::Mouse, &e);
    }
}

void Input::ProcessMouseMove(const Vec2i& position)
{
    sState.MousePos = position;

    MouseEvent e { MouseEventType::Move, Button::Middle, position };
    EventDispatcher::Dispatch(EventCategory::Mouse, &e);
}

void Input::PushMouseRawDelta(const Vec2i& delta)
{
    sState.RawDeltaBuffer.push(delta);

    if (sState.RawDeltaBuffer.size() > 16)
    {
        sState.RawDeltaBuffer.pop();
    }
}

bool Input::IsKeyDown(Key key)
{
    return sState.Keys[static_cast<i32>(key)];
}

bool Input::IsButtonDown(Button button)
{
    return sState.Buttons[static_cast<i32>(button)];
}

std::optional<Vec2i> Input::PopMouseDelta()
{
    if (sState.RawDeltaBuffer.empty())
        return std::nullopt;

    Vec2i out { sState.RawDeltaBuffer.front() };
    sState.RawDeltaBuffer.pop();
    return out;
}