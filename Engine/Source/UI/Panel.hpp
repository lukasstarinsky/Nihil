#pragma once

#include "Common/Event.hpp"
#include "Widget.hpp"

namespace UI
{

class Panel : public Widget
{
public:
    explicit Panel(Widget* parent);

    void OnInitialize() override;
    void Render() const override;

    bool OnKeyEvent(const KeyEvent& e);
private:
    MaterialInstancePtr mMaterial {};
};

}
