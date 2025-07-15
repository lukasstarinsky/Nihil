#include "Sandbox.hpp"

#define MOUSE_SENSITIVITY 0.008f

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
    mAssetManager = std::make_unique<RawAssetManager>("Assets/");
    mMesh = Mesh::Create(mAssetManager->LoadMesh("cottage_obj", "Cube_Cube.002"));
    mTexture = Texture::Create(mAssetManager->LoadTexture("container2"));
    mMaterial = mAssetManager->GetDefaultMaterial();

    MappedFile meshFile("Assets/Models/cottae_obj.obj");
    ADD_EVENT_LISTENER_THIS(Event::MouseMove, OnMouseMoveEvent);
    ADD_EVENT_LISTENER_THIS(Event::KeyPress, OnKeyPress);
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
    auto delta = e.MouseEvent.Delta * MOUSE_SENSITIVITY;
    mCamera.Rotate(-delta.y, delta.x);

    return false;
}

auto Sandbox::OnKeyPress(const Event& e) -> bool
{
    if (e.KeyEvent.Key == Key::F2)
    {
        mAssetManager->PackAll();
    }
    return true;
}