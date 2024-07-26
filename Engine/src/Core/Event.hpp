#pragma once

#include <functional>
#include "Defines.hpp"
#include "Input.hpp"

enum class EventCategory
{
    Application = 0,
    Key,
    Mouse,
    Total
};

enum class ApplicationEventType
{
    Quit = 0,
    Resize
};

enum class KeyEventType
{
    Press = 0,
    Release
};

enum class MouseEventType
{
    Press = 0,
    Release,
    Scroll,
    Move
};

struct ApplicationEvent
{
    ApplicationEventType Type;
    i32 Width {};
    i32 Height {};
};

struct KeyEvent
{
    KeyEventType Type;
    Key Key;
};

struct MouseEvent
{
    MouseEventType Type;
    Button Button;
    Vec2i Position {};
};

using Event = const void*;

namespace EventDispatcher
{
    using EventCallback = std::function<bool(Event)>;

    NIHIL_API void AddListener(EventCategory category, const EventCallback& callback);
    NIHIL_API void Dispatch(EventCategory category, Event e);
}

#define SET_EVENT_LISTENER_THIS(category, fn) EventDispatcher::AddListener(category, [this](Event e) -> bool { return fn(e); })
#define SET_EVENT_LISTENER(category, fn)      EventDispatcher::AddListener(category, fn)