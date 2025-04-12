#include "Sandbox.hpp"

#define MOUSE_SENSITIVITY 0.008f

static constexpr Vertex vertexData[] = {
    // Front face
    { .Position = {-0.5f, -0.5f,  0.5f}, .TexCoord = {0.0f, 0.0f} },
    { .Position = { 0.5f, -0.5f,  0.5f}, .TexCoord = {1.0f, 0.0f} },
    { .Position = { 0.5f,  0.5f,  0.5f}, .TexCoord = {1.0f, 1.0f} },
    { .Position = {-0.5f,  0.5f,  0.5f}, .TexCoord = {0.0f, 1.0f} },

    // Back face
    { .Position = { 0.5f, -0.5f, -0.5f}, .TexCoord = {0.0f, 0.0f} },
    { .Position = {-0.5f, -0.5f, -0.5f}, .TexCoord = {1.0f, 0.0f} },
    { .Position = {-0.5f,  0.5f, -0.5f}, .TexCoord = {1.0f, 1.0f} },
    { .Position = { 0.5f,  0.5f, -0.5f}, .TexCoord = {0.0f, 1.0f} },

    // Left face
    { .Position = {-0.5f, -0.5f, -0.5f}, .TexCoord = {0.0f, 0.0f} },
    { .Position = {-0.5f, -0.5f,  0.5f}, .TexCoord = {1.0f, 0.0f} },
    { .Position = {-0.5f,  0.5f,  0.5f}, .TexCoord = {1.0f, 1.0f} },
    { .Position = {-0.5f,  0.5f, -0.5f}, .TexCoord = {0.0f, 1.0f} },

    // Right face
    { .Position = { 0.5f, -0.5f,  0.5f}, .TexCoord = {0.0f, 0.0f} },
    { .Position = { 0.5f, -0.5f, -0.5f}, .TexCoord = {1.0f, 0.0f} },
    { .Position = { 0.5f,  0.5f, -0.5f}, .TexCoord = {1.0f, 1.0f} },
    { .Position = { 0.5f,  0.5f,  0.5f}, .TexCoord = {0.0f, 1.0f} },

    // Top face
    { .Position = {-0.5f,  0.5f,  0.5f}, .TexCoord = {0.0f, 0.0f} },
    { .Position = { 0.5f,  0.5f,  0.5f}, .TexCoord = {1.0f, 0.0f} },
    { .Position = { 0.5f,  0.5f, -0.5f}, .TexCoord = {1.0f, 1.0f} },
    { .Position = {-0.5f,  0.5f, -0.5f}, .TexCoord = {0.0f, 1.0f} },

    // Bottom face
    { .Position = {-0.5f, -0.5f, -0.5f}, .TexCoord = {0.0f, 0.0f} },
    { .Position = { 0.5f, -0.5f, -0.5f}, .TexCoord = {1.0f, 0.0f} },
    { .Position = { 0.5f, -0.5f,  0.5f}, .TexCoord = {1.0f, 1.0f} },
    { .Position = {-0.5f, -0.5f,  0.5f}, .TexCoord = {0.0f, 1.0f} },
};

static constexpr Index indexData[] = {
    0, 1, 2, 2, 3, 0,       // Front
    4, 5, 6, 6, 7, 4,       // Back
    8, 9,10,10,11, 8,       // Left
    12,13,14,14,15,12,      // Right
    16,17,18,18,19,16,      // Top
    20,21,22,22,23,20,      // Bottom
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
    mMesh = Mesh::Create(vertexData, indexData);
    mTexture = Texture::Create("Assets/Textures/container2.png");
    mMaterial = Renderer::DefaultMaterial();

    ADD_EVENT_LISTENER_THIS(Event::MouseMove, OnMouseMoveEvent);
}

void Sandbox::OnUpdate(f32 deltaTimeSeconds)
{
    mMaterial->SetUniform(0, Mat4f::Identity());

    if (Input::IsKeyDown(Key::W) || Input::IsKeyDown(Key::S) || Input::IsKeyDown(Key::A) || Input::IsKeyDown(Key::D))
    {
        Vec3f moveVector;
        moveVector += Input::IsKeyDown(Key::W) ? mCamera.Front() : Input::IsKeyDown(Key::S) ? -mCamera.Front() : Vec3f{};
        moveVector += Input::IsKeyDown(Key::A) ? -mCamera.Right() : Input::IsKeyDown(Key::D) ? mCamera.Right() : Vec3f{};
        moveVector.Normalize();
        mCamera.Translate(moveVector * deltaTimeSeconds * 5.0f);
    }
}

void Sandbox::OnResize()
{
    mCamera.SetAspectRatio(static_cast<f32>(Config.WindowWidth) / static_cast<f32>(Config.WindowHeight));
}

void Sandbox::OnRender()
{
    Renderer::BeginScene(mCamera);
    mTexture->Bind(0);
    mMaterial->Bind();
    Renderer::Draw(mMesh);
}

void Sandbox::OnShutdown()
{

}

auto Sandbox::OnMouseMoveEvent(const Event& e) -> bool
{
    auto delta = e.MouseEvent.Delta;
    delta *= MOUSE_SENSITIVITY;
    mCamera.Rotate(-delta.y, delta.x);

    return false;
}