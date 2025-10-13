#pragma once

#include "AssetPipeline/AssetManager.hpp"
#include "Graphics/Camera.hpp"
#include "Widget.hpp"

namespace UI
{

class Manager
{
public:
    explicit Manager(AssetManager* assetManager, Widget* root);
    ~Manager();

    auto GetCamera() const -> const Camera&;
    void OnResize(i32 width, i32 height);
    void Render() const;
private:
    AssetManager* mAssetManager {};
    MeshPtr mQuadMesh {};
    BufferPtr mQuadInstanceBuffer {};
    VertexLayout mInstanceVertexLayout {};

    Widget* mRootWidget {};
    Camera mCamera;
};

}