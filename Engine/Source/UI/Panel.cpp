#include "Panel.hpp"

#include "Graphics/Renderer.hpp"

namespace UI
{

Panel::Panel(Widget* parent)
    : Widget{parent}
{
}

auto Panel::OnMouseClick() const -> bool
{
    Widget::OnMouseClick();
    return true;
}

}