#pragma once

#include <vector>

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
    explicit Widget(Widget* parent = nullptr);
    virtual ~Widget();

    void AddWidget(Widget* widget);

    void SetPosition(const Vec2f& position);
    void SetSize(const Vec2f& size);

    auto CollectInstanceData(std::vector<WidgetInstanceData>& outData) const -> void;
protected:
    WidgetInstanceData mInstanceData {};

    Widget* mParent {};
    std::vector<Widget*> mChildren {};
};

}