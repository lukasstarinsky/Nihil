#pragma once

namespace UI
{

enum class AnchorType
{
    Left = 0,
    Right,
    Top,
    Bottom,
    Stretch,
};

struct Anchor
{
    void SetHorizontal(AnchorType type);
    void SetVertical(AnchorType type);
public:
    AnchorType Horizontal = AnchorType::Left;
    AnchorType Vertical = AnchorType::Top;
    Vec2f Min {};
    Vec2f Max {};
};

}