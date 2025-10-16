#include "Editor.hpp"

#define MOUSE_SENSITIVITY 0.008f

Editor::Editor()
    : mCamera{CameraProjection::Perspective, {0.0f, 0.0f, 5.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 90.0f, 1280.0f / 960.0f}
    , mAssetPipeline{"Assets/"}
{
    Config.WindowWidth = 1280;
    Config.WindowHeight = 960;
    Config.Name = "Nihil Editor";
    Config.RendererAPI = RendererAPI::OpenGL;
}

void Editor::OnInitialize()
{
    if (!mAssetPipeline.ValidateManifest())
    {
        Logger::Warn("Asset manifest is not valid, rebuilding all assets...");
        mAssetPipeline.BuildAll("Assets/01.npack", 1, MEGABYTE(2000));
    }

    mAssetManager = std::make_unique<AssetManager>("Assets/01.npack");

    auto* rootWidget = new UI::Panel(nullptr);
    rootWidget->SetPosition({100.0f, 100.0f});
    rootWidget->SetSize({400.0f, 300.0f});
    rootWidget->SetOnMouseClick([]() {
        Logger::Info("Root panel clicked");
    });

    auto* panel = new UI::Panel(rootWidget);
    panel->SetPosition({550.0f, 250.0f});
    panel->SetSize({200.0f, 150.0f});
    panel->SetOnMouseClick([]() {
        Logger::Info("Child panel clicked");
    });

    auto* panel2 = new UI::Panel(rootWidget);
    panel2->SetPosition({900.0f, 50.0f});
    panel2->SetSize({150.0f, 100.0f});
    panel2->SetOnMouseClick([]() {
        Logger::Info("Second child panel clicked");
    });

    mUIManager = std::make_unique<UI::Manager>(mAssetManager.get(), rootWidget);

    mMesh = mAssetManager->Get<Mesh>(mAssetPipeline.GetManifest().GetUUID("sponza"));
    EventDispatcher::AddListener<MouseEvent>(std::bind_front(&Editor::OnMouseEvent, this));
    EventDispatcher::AddListener<KeyEvent>(std::bind_front(&Editor::OnKeyEvent, this));
}

void Editor::OnUpdate(f32 deltaTimeSeconds)
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

void Editor::OnResize()
{
    mCamera.OnResize(Config.WindowWidth, Config.WindowHeight);
    mUIManager->OnResize(Config.WindowWidth, Config.WindowHeight);
}

void Editor::OnRender()
{
    Renderer::BeginScene(mCamera);
    Renderer::Draw(mMesh, Mat4f::Scale({0.02f, 0.02f, 0.02f}));
}

void Editor::OnUIRender()
{
    Renderer::BeginScene(mUIManager->GetCamera());
    mUIManager->Render();
}

void Editor::OnShutdown()
{

}

auto Editor::OnMouseEvent(const MouseEvent& e) -> bool
{
    if (e.Type == EventType::MouseMove)
    {
        auto delta = e.Delta * MOUSE_SENSITIVITY;
        mCamera.Rotate(-delta.y, delta.x);
    }
    return false;
}

auto Editor::OnKeyEvent(const KeyEvent& e) -> bool
{
    if (e.Type == EventType::KeyPress)
    {
        if (e.Key == Key::F2)
        {
            mUIManager->GetRootWidget()->Toggle();
        }
    }

    return true;
}