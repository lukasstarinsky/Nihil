#pragma once

#include "AssetPipeline/AssetManager.hpp"
#include "Graphics/Camera.hpp"
#include "Common/Event.hpp"
#include "Widget.hpp"

namespace UI
{

class Manager
{
public:
    explicit Manager(AssetManager* assetManager, Widget* root);
    ~Manager();

    auto GetRootWidget() const -> Widget*;
    auto GetCamera() const -> const Camera&;
    void OnResize(i32 width, i32 height);
    void Render() const;

    auto OnMouseEvent(const MouseEvent& event) const -> bool;
private:
    AssetManager* mAssetManager {};
    MeshPtr mQuadMesh {};
    BufferPtr mQuadInstanceBuffer {};
    VertexLayout mInstanceVertexLayout {};

    Widget* mRootWidget {};
    Camera mCamera;
};

}