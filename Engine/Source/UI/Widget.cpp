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

auto Widget::HitTest(const Vec2f& point) const -> bool
{
    return GetAbsoluteRect().Contains(point);
}

auto Widget::CollectInstanceData(std::vector<WidgetInstanceData>& outData) const -> void
{
    outData.push_back({ .Position = GetAbsoluteRect().Position, .Size = mRect.Size });
    for (const auto* child: mChildren)
    {
        child->CollectInstanceData(outData);
    }
}

auto Widget::GetAbsoluteRect() const -> Rect
{
//    if (mParent)
//    {
//        Rect parentRect = mParent->GetAbsoluteRect();
//        return {
//            parentRect.Position + mRect.Position,
//            mRect.Size
//        };
//    }
    return mRect;
}

auto Widget::GetLastWidgetAt(const Vec2f& point) const -> const Widget*
{
    for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it)
    {
        auto* child = *it;
        if (child->HitTest(point))
        {
            const auto* hit = child->GetLastWidgetAt(point);
            if (hit)
                return hit;
            return child;
        }
    }
    if (HitTest(point))
        return this;
    return nullptr;
}

void Widget::SetOnMouseClick(const UI::Widget::EventCallback& callback)
{
    mOnMouseClick = callback;
}

void Widget::SetPosition(const Vec2f& position)
{
    mRect.Position = position;
}

void Widget::SetSize(const Vec2f& size)
{
    mRect.Size = size;
}

}