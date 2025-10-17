#include "Layout.hpp"

namespace UI
{

Layout::Layout(LayoutType type, Widget* parent)
    : Widget{parent}
    , mType{type}
{
    SetRenderable(false);
}

void Layout::Update(f32 deltaTimeSeconds)
{
    if (mIsDirty)
    {
        if (mType == LayoutType::Vertical)
        {
            auto y = mPadding;
            for (auto* child: mChildren)
            {
                child->SetPosition({ mPadding, y });
                y += child->GetSize().y + mSpacing;
            }
        }
        else if (mType == LayoutType::Horizontal)
        {
            auto x = mPadding;
            for (auto* child: mChildren)
            {
                child->SetPosition({ x, mPadding });
                x += child->GetSize().x + mSpacing;
            }
        }
    }

    Widget::Update(deltaTimeSeconds);
}

void Layout::SetSpacing(f32 spacing)
{
    mSpacing = spacing;
}

void Layout::SetPadding(f32 padding)
{
    mPadding = padding;
}

}