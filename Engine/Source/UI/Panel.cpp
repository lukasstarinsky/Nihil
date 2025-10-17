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

auto Panel::SetLayout(LayoutType type) -> Layout*
{
    auto* layout = new Layout(type, this);
    layout->SetHorizontalAnchor(AnchorType::Stretch);
    layout->SetVerticalAnchor(AnchorType::Stretch);
    return layout;
}

}