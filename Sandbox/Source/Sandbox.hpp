#pragma once

#include <Nihil.hpp>

class Sandbox : public Application
{
public:
    Sandbox();

    void OnInitialize() override;
    void OnUpdate(f32 deltaTimeSeconds) override;
    void OnRender() override;
    void OnShutdown() override;
private:
    MeshPtr mTestMesh {};
    Camera mCamera;
};