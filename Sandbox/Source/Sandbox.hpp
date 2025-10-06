#pragma once

#include <Nihil.hpp>

class Sandbox : public Application
{
public:
    Sandbox();

    void OnInitialize() override;
    void OnUpdate(f32 deltaTimeSeconds) override;
    void OnResize() override;
    void OnRender() override;
    void OnShutdown() override;

    auto OnMouseEvent(const MouseEvent& e) -> bool;
    auto OnKeyEvent(const KeyEvent& e) -> bool;
private:
    MeshPtr mMesh {};
    MaterialPtr mMaterial {};
    TexturePtr mTexture {};
    Camera mCamera;

    AssetPipeline mAssetPipeline;
    std::unique_ptr<AssetManager> mAssetManager {};
};