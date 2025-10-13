#pragma once

#include "AssetPipeline/AssetManager.hpp"
#include "Graphics/Camera.hpp"
#include "Widget.hpp"

namespace UI
{

class Manager
{
public:
    explicit Manager(AssetManager* assetManager);
    ~Manager();

    auto GetCamera() const -> const Camera&;
    void SetRootWidget(Widget* root);
    void OnResize(i32 width, i32 height);
    void Render() const;
private:
    AssetManager* mAssetManager {};
    Widget* mRootWidget {};
    Camera mCamera;
};

}