#pragma once

#include <Nihil.hpp>

class Editor : public Application
{
public:
    Editor();

    void OnInitialize() override;
    void OnUpdate(f32 deltaTimeSeconds) override;
    void OnWindowResize() override;
    void OnRender() override;
    void OnUIRender() override;
    void OnShutdown() override;

    auto OnMouseEvent(const MouseEvent& e) -> bool;
    auto OnKeyEvent(const KeyEvent& e) -> bool;
private:
    MeshPtr mMesh {};
    Camera mCamera;

    AssetPipeline mAssetPipeline;
    std::unique_ptr<AssetManager> mAssetManager {};
    std::unique_ptr<UI::Manager> mUIManager {};
};