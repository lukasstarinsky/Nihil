#include "Anchor.hpp"

namespace UI
{

void Anchor::SetHorizontal(UI::AnchorType type)
{
    switch (type)
    {
        case AnchorType::Left:
            Min.x = 0.0f;
            Max.x = 0.0f;
            break;
        case AnchorType::Right:
            Min.x = 1.0f;
            Max.x = 1.0f;
            break;
        case AnchorType::Stretch:
            Min.x = 0.0f;
            Max.x = 1.0f;
            break;
        default:
            break;
    }
}

void Anchor::SetVertical(UI::AnchorType type)
{
    switch (type)
    {
        case AnchorType::Top:
            Min.y = 0.0f;
            Max.y = 0.0f;
            break;
        case AnchorType::Bottom:
            Min.y = 1.0f;
            Max.y = 1.0f;
            break;
        case AnchorType::Stretch:
            Min.y = 0.0f;
            Max.y = 1.0f;
            break;
        default:
            break;
    }
}

}