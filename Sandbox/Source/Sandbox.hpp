#pragma once

#include <Nihil.hpp>

class Sandbox : public Application
{
public:
    Sandbox();

    void OnInitialize() override;
    void OnUpdate() override;
    void OnRender() override;
    void OnShutdown() override;
private:
    MeshPtr mTestMesh {};
};