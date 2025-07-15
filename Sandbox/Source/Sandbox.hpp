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

    auto OnMouseMoveEvent(const Event& e) -> bool;
    auto OnKeyPress(const Event& e) -> bool;
private:
    MeshPtr mMesh {};
    MaterialPtr mMaterial {};
    TexturePtr mTexture {};
    Camera mCamera;
};