#pragma once

#include <vector>
#include <functional>

#include "AssetPipeline/AssetManager.hpp"
#include "Math/Math.hpp"
#include "Anchor.hpp"

namespace UI
{

enum class SizePolicy
{
    Fixed = 0,
    Expand,
    Stretch
};

struct WidgetInstanceData
{
    Vec2f Position {};
    Vec2f Size {};
    Vec4f Color {};
};

class Widget
{
public:
    using EventCallback = std::function<void()>;

    explicit Widget(Widget* parent = nullptr);
    virtual ~Widget();

    virtual void Update(f32 deltaTimeSeconds);
    virtual auto OnMouseClick() const -> bool;
    virtual void OnWindowResize(i32 width, i32 height);

    void AddWidget(Widget* widget);
    auto HitTest(const Vec2f& point) const -> bool;
    auto CollectInstanceData(std::vector<WidgetInstanceData>& outData) const -> void;
    void UpdateAnchors(const Vec2f& parentSize);

    auto GetSize() const -> Vec2f;
    auto GetWidth() const -> f32;
    auto GetHeight() const -> f32;
    auto GetAbsoluteRect() const -> Rect;
    auto GetLastWidgetAt(const Vec2f& point) const -> const Widget*;
    auto GetSizePolicy() const -> std::pair<SizePolicy, SizePolicy>;
    auto IsVisible() const -> bool;
    auto IsRenderable() const -> bool;
    void SetOnMouseClick(const EventCallback& callback);
    void SetPosition(const Vec2f& position);
    void SetSize(const Vec2f& size);
    void SetWidth(f32 width);
    void SetHeight(f32 height);
    void SetRenderable(bool renderable);
    void SetColor(const Vec4f& color);
    void SetVisible(bool visible);
    void SetHorizontalAnchor(AnchorType type);
    void SetVerticalAnchor(AnchorType type);
    void SetSizePolicy(SizePolicy widthPolicy, SizePolicy heightPolicy);
    void MarkDirty();

    template <typename T>
    auto MarkParentDirty() -> bool
    {
        auto* parent = dynamic_cast<T*>(mParent);
        while (parent)
        {
            if (auto newParent = dynamic_cast<T*>(parent->mParent))
            {
                parent = newParent;
            }
            else
            {
                parent->MarkDirty();
                return true;
            }
        }
        return false;
    }
protected:
    Rect mRect {};
    Widget* mParent {};
    Vec2f mNullParentSize {};
    std::vector<Widget*> mChildren {};
    EventCallback mOnMouseClick {};
    Vec4f mColor {};
    Anchor mAnchor {};
    SizePolicy mWidthPolicy = SizePolicy::Fixed;
    SizePolicy mHeightPolicy = SizePolicy::Fixed;
    bool mUseAnchors = false;
    bool mIsDirty = true;
    bool mVisible = true;
    bool mRenderable = true;
};

}