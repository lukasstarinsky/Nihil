#pragma once

#include <vector>
#include <functional>

#include "AssetPipeline/AssetManager.hpp"
#include "Math/Math.hpp"

namespace UI
{

struct WidgetInstanceData
{
    Vec2f Position {};
    Vec2f Size {};
};

class Widget
{
public:
    using EventCallback = std::function<void()>;

    explicit Widget(Widget* parent = nullptr);
    virtual ~Widget();

    virtual auto OnMouseClick() const -> bool = 0;

    void AddWidget(Widget* widget);
    auto HitTest(const Vec2f& point) const -> bool;
    auto CollectInstanceData(std::vector<WidgetInstanceData>& outData) const -> void;

    auto GetAbsoluteRect() const -> Rect;
    auto GetLastWidgetAt(const Vec2f& point) const -> const Widget*;
    void SetOnMouseClick(const EventCallback& callback);
    void SetPosition(const Vec2f& position);
    void SetSize(const Vec2f& size);
protected:
    Rect mRect {};
    Widget* mParent {};
    std::vector<Widget*> mChildren {};
    EventCallback mOnMouseClick {};
};

}