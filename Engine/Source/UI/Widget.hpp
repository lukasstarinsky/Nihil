#pragma once

#include <vector>

#include "AssetPipeline/AssetManager.hpp"
#include "Math/Math.hpp"

namespace UI
{

class Widget
{
public:
    explicit Widget(Widget* parent = nullptr);
    virtual ~Widget();

    virtual void OnInitialize() = 0;
    virtual void Render() const;

    void InitializeDefaultResources();
    void AddWidget(Widget* widget);
    void SetAssetManager(AssetManager* assetManager);
protected:
    Widget* mParent {};
    AssetManager* mAssetManager {};
    std::vector<Widget*> mChildren {};

    MeshPtr mQuadMesh {};
    MaterialPtr mUIBaseMaterial {};
};

}