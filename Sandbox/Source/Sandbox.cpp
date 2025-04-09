#include "Sandbox.hpp"

Sandbox::Sandbox()
{
    Config.WindowWidth = 1280;
    Config.WindowHeight = 960;
    Config.Name = "Nihil Sandbox";
    Config.RendererAPI = RendererAPI::OpenGL;
}

void Sandbox::OnInitialize()
{
    mTestMesh = Mesh::Create();
}

void Sandbox::OnUpdate()
{

}

void Sandbox::OnRender()
{
    Renderer::Draw(mTestMesh);
}

void Sandbox::OnShutdown()
{

}