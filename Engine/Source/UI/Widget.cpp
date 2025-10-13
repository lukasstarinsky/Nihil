#include "Widget.hpp"

namespace UI
{

Widget::Widget(UI::Widget* parent)
    : mParent{parent}
{
    if (parent)
    {
        parent->AddWidget(this);
    }
}

Widget::~Widget()
{
    for (auto* child: mChildren)
    {
        delete child;
    }
}

void Widget::AddWidget(UI::Widget* widget)
{
    mChildren.push_back(widget);
    widget->mParent = this;
}

void Widget::SetPosition(const Vec3f& position)
{
    mInstanceData.Position = position;
}

void Widget::SetSize(const Vec3f& size)
{
    mInstanceData.Size = size;
}

auto Widget::CollectInstanceData(std::vector<WidgetInstanceData>& outData) const -> void
{
    outData.push_back(mInstanceData);
    for (const auto* child: mChildren)
    {
        child->CollectInstanceData(outData);
    }
}

}