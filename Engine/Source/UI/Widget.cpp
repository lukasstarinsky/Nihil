#include "Widget.hpp"

#include "Layout.hpp"

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

auto Widget::OnMouseClick() const -> bool
{
    if (mOnMouseClick)
    {
        mOnMouseClick();
        return true;
    }
    return false;
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
    if (!mVisible)
        return;

    if (true)
    {
        outData.push_back({ .Position = GetAbsoluteRect().Position, .Size = mRect.Size, .Color = mColor });
    }
    for (const auto* child: mChildren)
    {
        child->CollectInstanceData(outData);
    }
}

auto Widget::GetSize() const -> Vec2f
{
    return mRect.Size;
}

auto Widget::GetAbsoluteRect() const -> Rect
{
    if (mParent)
    {
        Rect parentRect = mParent->GetAbsoluteRect();
        return {
            parentRect.Position + mRect.Position,
            mRect.Size
        };
    }
    return mRect;
}

auto Widget::GetLastWidgetAt(const Vec2f& point) const -> const Widget*
{
    if (!mVisible)
        return nullptr;

    for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it)
    {
        auto* child = *it;
        if (child->HitTest(point) && child->mVisible)
        {
            const auto* hit = child->GetLastWidgetAt(point);
            if (hit && hit->mVisible)
                return hit;
            return child;
        }
    }
    if (HitTest(point))
        return this;
    return nullptr;
}

auto Widget::IsVisible() const -> bool
{
    return mVisible;
}

auto Widget::IsRenderable() const -> bool
{
    return mRenderable;
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

void Widget::SetRenderable(bool renderable)
{
    mRenderable = renderable;
}

void Widget::SetColor(const Vec4f& color)
{
    mColor = color;
}

void Widget::SetVisible(bool visible)
{
    mVisible = visible;

    for (auto* child: mChildren)
    {
        child->SetVisible(visible);
    }
}

}