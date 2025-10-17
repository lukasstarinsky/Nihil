#pragma once

#include "Widget.hpp"

namespace UI
{

enum class LayoutType
{
    Vertical = 0,
    Horizontal
};

class Layout : public Widget
{
public:
    explicit Layout(LayoutType type, Widget* parent = nullptr);

    void Update(f32 deltaTimeSeconds) override;
    void SetSpacing(f32 spacing);
    void SetPadding(f32 padding);
private:
    LayoutType mType {};
    f32 mSpacing = 5.0f;
    f32 mPadding = 5.0f;
};

}