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

static constexpr Index indexData[] = {
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
    // Projection, Position, LookAt, Up, Fov, AspectRatio
    : mCamera{CameraProjection::Perspective, {0.0f, 0.0f, 5.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 90.0f, 800.0f / 600.0f}
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

void Sandbox::OnUpdate(f32 deltaTimeSeconds)
{
    static f32 theta = 0.01f;
    theta += 0.01f;
    mTestMesh->GetMaterial()->SetUniform(0, Mat4f::RotateX(theta));

    if (Input::IsKeyDown(Key::W) || Input::IsKeyDown(Key::S) || Input::IsKeyDown(Key::A) || Input::IsKeyDown(Key::D))
    {
        Vec3f moveVector;
        moveVector.z = Input::IsKeyDown(Key::W) ? -1.0f : Input::IsKeyDown(Key::S) ? 1.0f : 0.0f;
        moveVector.x = Input::IsKeyDown(Key::A) ? -1.0f : Input::IsKeyDown(Key::D) ? 1.0f : 0.0f;
        moveVector.Normalize();
        mCamera.Translate(moveVector * deltaTimeSeconds * 5.0f);
    }
}

void Sandbox::OnRender()
{
    Renderer::BeginScene(mCamera);
    Renderer::Draw(mTestMesh);
}

void Sandbox::OnShutdown()
{

}