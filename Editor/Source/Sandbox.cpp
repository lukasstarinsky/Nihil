#include "Sandbox.hpp"

#define MOUSE_SENSITIVITY 0.008f

Sandbox::Sandbox()
    : mCamera{CameraProjection::Perspective, {0.0f, 0.0f, 5.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 90.0f, 800.0f / 600.0f}
    , mAssetPipeline{"Assets/"}
{
    Config.WindowWidth = 1280;
    Config.WindowHeight = 960;
    Config.Name = "Nihil Editor";
    Config.RendererAPI = RendererAPI::OpenGL;
}

void Sandbox::OnInitialize()
{
    if (!mAssetPipeline.ValidateManifest())
    {
        Logger::Warn("Asset manifest is not valid, rebuilding all assets...");
        mAssetPipeline.BuildAll("Assets/01.npack", 1, MEGABYTE(1));
    }

    mAssetManager = std::make_unique<AssetManager>("Assets/01.npack");
//    mUIManager = std::make_unique<UI::Manager>(mAssetManager.get());
//
//    auto* button = new UI::Button(nullptr);
//    mUIManager->SetRootWidget(button);

    mMesh = mAssetManager->Get<Mesh>(mAssetPipeline.GetManifest().GetUUID("sponza"));
    EventDispatcher::AddListener<MouseEvent>(std::bind_front(&Sandbox::OnMouseEvent, this));
    EventDispatcher::AddListener<KeyEvent>(std::bind_front(&Sandbox::OnKeyEvent, this));
}

void Sandbox::OnUpdate(f32 deltaTimeSeconds)
{
    if (Input::IsKeyDown(Key::W) || Input::IsKeyDown(Key::S) || Input::IsKeyDown(Key::A) || Input::IsKeyDown(Key::D))
    {
        Vec3f moveVector;
        moveVector += Input::IsKeyDown(Key::W) ? mCamera.Front() : Input::IsKeyDown(Key::S) ? -mCamera.Front() : Vec3f{};
        moveVector += Input::IsKeyDown(Key::A) ? -mCamera.Right() : Input::IsKeyDown(Key::D) ? mCamera.Right() : Vec3f{};
        moveVector.Normalize();
        mCamera.Translate(moveVector * deltaTimeSeconds * 15.0f);
    }
}

void Sandbox::OnResize()
{
    mCamera.SetAspectRatio(static_cast<f32>(Config.WindowWidth) / static_cast<f32>(Config.WindowHeight));
}

void Sandbox::OnRender()
{
    Renderer::BeginScene(mCamera);
    Renderer::Draw(mMesh, Mat4f::Scale({0.02f, 0.02f, 0.02f}));

//    mUIManager->Render();
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
    return true;
}