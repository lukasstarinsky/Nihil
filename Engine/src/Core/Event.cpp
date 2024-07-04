#include "Event.hpp"

static std::vector<EventDispatcher::EventCallback> sEventListeners[static_cast<i32>(EventCategory::Total)];

void EventDispatcher::AddListener(EventCategory category, const EventDispatcher::EventCallback& callback)
{
    sEventListeners[static_cast<i32>(category)].push_back(callback);
}

void EventDispatcher::Dispatch(EventCategory category, Event e)
{
    if (!e || sEventListeners[static_cast<i32>(category)].empty())
        return;

    for (const auto& callback : sEventListeners[static_cast<i32>(category)])
    {
        if (callback(e))
            return;
    }
}