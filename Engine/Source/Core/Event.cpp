#include "Event.hpp"

std::vector<std::function<bool(const Event&)>> EventDispatcher::sEventListeners;