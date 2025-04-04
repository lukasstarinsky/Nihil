#include "Event.hpp"

static std::vector<std::pair<u64, EventDispatcher::EventCallback>> sEventListeners;

void EventDispatcher::AddListener(u64 flag, const EventCallback& callback)
{
    sEventListeners.emplace_back(flag, callback);
}

void EventDispatcher::Dispatch(const Event& e)
{
    for (const auto& [flag, callback]: sEventListeners)
    {
        if ((flag & e.Type) && callback(e))
            return;
    }
}