#pragma once

#include <functional>
#include "Defines.hpp"
#include "Input.hpp"

struct ApplicationEvent
{
    i32 Width {};
    i32 Height {};
};

struct MouseEvent
{
    Button Button { Button::Middle };
    Vec2i Position {};
};

struct Event
{
    enum EventType
    {
        ApplicationQuit     = 1 << 0,
        ApplicationResize   = 1 << 1,

        KeyPress            = 1 << 2,
        KeyRelease          = 1 << 3,

        MousePress          = 1 << 4,
        MouseRelease        = 1 << 5,
        MouseScroll         = 1 << 6,
        MouseMove           = 1 << 7
    };

    EventType Type;
    union
    {
        struct ApplicationEvent ApplicationEvent;
        struct MouseEvent MouseEvent;
        Key KeyEvent;
    };
};

namespace EventDispatcher
{
    using EventCallback = std::function<bool(const Event&)>;

    NIHIL_API void AddListener(u64 flag, const EventCallback& callback);
    NIHIL_API void Dispatch(const Event& e);
}

#define ADD_EVENT_LISTENER_THIS(flag, fn) EventDispatcher::AddListener(flag, [this](const Event& e) -> bool { return fn(e); })
#define ADD_EVENT_LISTENER(flag, fn)      EventDispatcher::AddListener(flag, fn)