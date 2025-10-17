#pragma once

#include "Common/Event.hpp"
#include "Widget.hpp"
#include "Layout.hpp"

namespace UI
{

class Panel : public Widget
{
public:
    explicit Panel(Widget* parent);

    auto OnMouseClick() const -> bool override;

    auto SetLayout(LayoutType type) -> Layout*;
};

}
