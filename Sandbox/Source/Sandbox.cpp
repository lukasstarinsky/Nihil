#include "Sandbox.hpp"

#define MOUSE_SENSITIVITY 0.008f

Sandbox::Sandbox()
    : mCamera{CameraProjection::Perspective, {0.0f, 0.0f, 5.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 90.0f, 800.0f / 600.0f}
{
    Config.WindowWidth = 1280;
    Config.WindowHeight = 960;
    Config.Name = "Nihil Sandbox";
    Config.RendererAPI = RendererAPI::OpenGL;
}

void Sandbox::OnInitialize()
{
    try
    {
        mAssetManager = std::make_unique<PackedAssetManager>("Assets/01.npack");
    }
    catch (const NihilException& e)
    {
        Logger::Warn("{}", e.what());
        mAssetManager = std::make_unique<RawAssetManager>("Assets/");
    }
    mMesh = Mesh::Create(mAssetManager->LoadMesh("cottage_obj", "Cube_Cube.002"));
    mTexture = Texture::Create(mAssetManager->LoadTexture("container2"));
    mMaterial = mAssetManager->GetDefaultMaterial();

    EventDispatcher::AddListener<MouseEvent>(std::bind_front(&Sandbox::OnMouseEvent, this));
    EventDispatcher::AddListener<KeyEvent>(std::bind_front(&Sandbox::OnKeyEvent, this));
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

auto Sandbox::OnMouseEvent(const MouseEvent& e) -> bool
{
    if (e.Type == EventType::MouseMove)
    {
        auto delta = e.Delta * MOUSE_SENSITIVITY;
        mCamera.Rotate(-delta.y, delta.x);
    }
    return false;
}

auto Sandbox::OnKeyEvent(const KeyEvent& e) -> bool
{
    if (e.Type == EventType::KeyPress && e.Key == Key::F2)
    {
        RawAssetManager assetManager("Assets/");
        assetManager.PackAll("01.npack", 1, MEGABYTE(2));
    }
    return true;
}