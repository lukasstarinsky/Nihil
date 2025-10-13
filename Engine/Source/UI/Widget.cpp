#include "Widget.hpp"

namespace UI
{

Widget::Widget(UI::Widget* parent)
    : mParent{parent}
{
    if (parent)
    {
        parent->AddWidget(this);
        mAssetManager = parent->mAssetManager;
    }
}

Widget::~Widget()
{
    for (auto* child: mChildren)
    {
        delete child;
    }
}

void Widget::Render() const
{
    for (const auto* child: mChildren)
    {
        child->Render();
    }
}

void Widget::InitializeDefaultResources()
{
    mQuadMesh = mAssetManager->Get<Mesh>(DefaultResource::QuadMesh);
    mUIBaseMaterial = mAssetManager->Get<Material>(DefaultResource::UIMaterial);
}

void Widget::AddWidget(UI::Widget* widget)
{
    mChildren.push_back(widget);
    widget->mParent = this;
}

void Widget::SetAssetManager(AssetManager* assetManager)
{
    mAssetManager = assetManager;

    if (!mQuadMesh || !mUIBaseMaterial)
    {
        InitializeDefaultResources();
    }

    OnInitialize();
    for (auto* child: mChildren)
    {
        child->SetAssetManager(assetManager);
    }
}

}