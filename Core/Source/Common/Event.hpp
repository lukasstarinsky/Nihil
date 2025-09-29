#pragma once

#include <functional>
#include <variant>
#include "Defines.hpp"
#include "Input.hpp"

enum class EventType
{
    ApplicationQuit,
    ApplicationResize,

    MousePress,
    MouseRelease,
    MouseScroll,
    MouseMove,

    KeyPress,
    KeyRelease
};

struct ApplicationEvent
{
    i32 Width {};
    i32 Height {};
    EventType Type = EventType::ApplicationQuit;
};

struct MouseEvent
{
    Button Button {};
    Vec2f Delta {};
    EventType Type = EventType::MousePress;
};

struct KeyEvent
{
    Key Key {};
    EventType Type = EventType::KeyPress;
};

using Event = std::variant<ApplicationEvent, MouseEvent, KeyEvent>;

class NIHIL_API EventDispatcher
{
public:
    template<typename T>
    using EventCallback = std::function<bool(const T&)>;

    template<typename T>
    static void AddListener(const EventCallback<T>& callback)
    {
        auto wrapper = [callback](const Event& e) -> bool
        {
            if (const auto* eventPtr = std::get_if<T>(&e))
                return callback(*eventPtr);
            return false;
        };
        sEventListeners.push_back(wrapper);
    }

    static void Dispatch(const Event& e)
    {
        for (const auto& callback: sEventListeners)
        {
            if (callback(e))
                return;
        }
    }
private:
    static std::vector<std::function<bool(const Event&)>> sEventListeners;
};