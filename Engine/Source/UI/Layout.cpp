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
    Widget::Update(deltaTimeSeconds);

    if (mIsDirty)
    {
        if (mType == LayoutType::Vertical)
        {
            auto verticalStretchCount = 0;
            auto totalFixedHeight = 0.0f;

            for (auto* child : mChildren)
            {
                const auto [_, heightPolicy] = child->GetSizePolicy();
                if (heightPolicy == SizePolicy::Fixed)
                {
                    totalFixedHeight += child->GetHeight();
                }
                else if (heightPolicy == SizePolicy::Stretch)
                {
                    verticalStretchCount++;
                }
            }

            auto heightAvailable = GetHeight() - 2 * mPadding - mSpacing * (static_cast<f32>(mChildren.size()) - 1) - totalFixedHeight;
            if (heightAvailable < 0.0f)
            {
                heightAvailable = 0.0f;
            }

            auto currentY = mPadding;
            for (auto* child: mChildren)
            {
                const auto [widthPolicy, heightPolicy] = child->GetSizePolicy();

                if (widthPolicy == SizePolicy::Stretch)
                {
                    child->SetWidth(GetWidth() - 2 * mPadding);
                }
                if (heightPolicy == SizePolicy::Stretch)
                {
                    child->SetHeight(heightAvailable / static_cast<f32>(verticalStretchCount));
                }

                child->SetPosition({ mPadding, currentY });
                currentY += child->GetHeight() + mSpacing;
            }
        }
        else if (mType == LayoutType::Horizontal)
        {
            auto horizontalStretchCount = 0;
            auto totalFixedWidth = 0.0f;

            for (auto* child : mChildren)
            {
                const auto [widthPolicy, _] = child->GetSizePolicy();
                if (widthPolicy == SizePolicy::Fixed)
                {
                    totalFixedWidth += child->GetWidth();
                }
                else if (widthPolicy == SizePolicy::Stretch)
                {
                    horizontalStretchCount++;
                }
            }

            auto widthAvailable = GetWidth() - 2 * mPadding - mSpacing * (static_cast<f32>(mChildren.size()) - 1) - totalFixedWidth;
            if (widthAvailable < 0.0f)
            {
                widthAvailable = 0.0f;
            }

            auto currentX = mPadding;
            for (auto* child : mChildren)
            {
                const auto [widthPolicy, heightPolicy] = child->GetSizePolicy();

                if (widthPolicy == SizePolicy::Stretch && horizontalStretchCount > 0)
                {
                    child->SetWidth(widthAvailable / static_cast<f32>(horizontalStretchCount));
                }
                if (heightPolicy == SizePolicy::Stretch)
                {
                    child->SetHeight(GetHeight() - 2 * mPadding);
                }

                child->SetPosition({ currentX, mPadding });
                currentX += child->GetWidth() + mSpacing;
            }
        }

        mIsDirty = false;
    }
}

auto Layout::GetSpacing() const -> f32
{
    return mSpacing;
}

auto Layout::GetPadding() const -> f32
{
    return mPadding;
}

void Layout::SetSpacing(f32 spacing)
{
    mSpacing = spacing;
    MarkDirty();
}

void Layout::SetPadding(f32 padding)
{
    mPadding = padding;
    MarkDirty();
}

}