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
    // Top-Left default
    Vec2f Min {};
    Vec2f Max {};
};

}