#pragma once

#include "Common/Event.hpp"
#include "Widget.hpp"

namespace UI
{

class Panel : public Widget
{
public:
    explicit Panel(Widget* parent);
};

}
