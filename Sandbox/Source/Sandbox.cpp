#include "Sandbox.hpp"

static constexpr Vertex vertexData[] = {
    // Front face
    { .Position = {-0.5f, -0.5f,  0.5f}, .Color = {1.0f, 0.0f, 0.0f} },
    { .Position = { 0.5f, -0.5f,  0.5f}, .Color = {0.0f, 1.0f, 0.0f} },
    { .Position = { 0.5f,  0.5f,  0.5f}, .Color = {0.0f, 0.0f, 1.0f} },
    { .Position = {-0.5f,  0.5f,  0.5f}, .Color = {1.0f, 1.0f, 0.0f} },

    // Back face
    { .Position = {-0.5f, -0.5f, -0.5f}, .Color = {1.0f, 0.0f, 1.0f} },
    { .Position = { 0.5f, -0.5f, -0.5f}, .Color = {0.0f, 1.0f, 1.0f} },
    { .Position = { 0.5f,  0.5f, -0.5f}, .Color = {1.0f, 0.5f, 0.0f} },
    { .Position = {-0.5f,  0.5f, -0.5f}, .Color = {0.5f, 0.0f, 1.0f} },
};

static constexpr u32 indexData[] = {
    // Front face
    0, 1, 2,
    2, 3, 0,

    // Right face
    1, 5, 6,
    6, 2, 1,

    // Back face
    5, 4, 7,
    7, 6, 5,

    // Left face
    4, 0, 3,
    3, 7, 4,

    // Top face
    3, 2, 6,
    6, 7, 3,

    // Bottom face
    4, 5, 1,
    1, 0, 4
};

Sandbox::Sandbox()
{
    Config.WindowWidth = 1280;
    Config.WindowHeight = 960;
    Config.Name = "Nihil Sandbox";
    Config.RendererAPI = RendererAPI::OpenGL;
}

void Sandbox::OnInitialize()
{
    mTestMesh = Mesh::Create(vertexData, indexData);
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