#pragma once

#include <vector>
#include <functional>

#include "AssetPipeline/AssetManager.hpp"
#include "Math/Math.hpp"
#include "Anchor.hpp"

namespace UI
{

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
    auto GetAbsoluteRect() const -> Rect;
    auto GetLastWidgetAt(const Vec2f& point) const -> const Widget*;
    auto IsVisible() const -> bool;
    auto IsRenderable() const -> bool;
    void SetOnMouseClick(const EventCallback& callback);
    void SetPosition(const Vec2f& position);
    void SetSize(const Vec2f& size);
    void SetRenderable(bool renderable);
    void SetColor(const Vec4f& color);
    void SetVisible(bool visible);
    void SetHorizontalAnchor(AnchorType type);
    void SetVerticalAnchor(AnchorType type);
    void MarkDirty();
protected:
    Rect mRect {};
    Widget* mParent {};
    std::vector<Widget*> mChildren {};
    EventCallback mOnMouseClick {};
    Vec4f mColor {};
    Anchor mAnchor {};
    bool mUseAnchors = false;
    bool mIsDirty = true;
    bool mVisible = true;
    bool mRenderable = true;
};

}