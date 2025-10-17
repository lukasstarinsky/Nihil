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

UI::Panel* leftPanel = nullptr;

void Editor::OnInitialize()
{
    if (!mAssetPipeline.ValidateManifest())
    {
        Logger::Warn("Asset manifest is not valid, rebuilding all assets...");
        mAssetPipeline.BuildAll("Assets/01.npack", 1, MEGABYTE(2000));
    }

    mAssetManager = std::make_unique<AssetManager>("Assets/01.npack");

    auto* root = new UI::Layout(UI::LayoutType::Horizontal);
    root->SetVerticalAnchor(UI::AnchorType::Stretch);
    root->SetHorizontalAnchor(UI::AnchorType::Stretch);

    leftPanel = new UI::Panel(root);
    leftPanel->SetSizePolicy(UI::SizePolicy::Fixed, UI::SizePolicy::Stretch);
    leftPanel->SetWidth(300.0f);
    leftPanel->SetColor({1.0f, 0.0f, 0.0f, 1.0f});
    auto* leftLayout = leftPanel->SetLayout(UI::LayoutType::Vertical);

    for (i32 i = 0; i < 5; i++)
    {
        auto* p = new UI::Panel(leftLayout);
        p->SetSizePolicy(UI::SizePolicy::Stretch, UI::SizePolicy::Stretch);
    }

    auto* spacer = new UI::Panel(root);
    spacer->SetSizePolicy(UI::SizePolicy::Stretch, UI::SizePolicy::Stretch);
    spacer->SetRenderable(false);

    auto* rightPanel = new UI::Panel(root);
    rightPanel->SetSizePolicy(UI::SizePolicy::Fixed, UI::SizePolicy::Stretch);
    rightPanel->SetWidth(300.0f);
    rightPanel->SetColor({0.0f, 0.0f, 1.0f, 1.0f});

    mUIManager = std::make_unique<UI::Manager>(mAssetManager.get(), root);

    mMesh = mAssetManager->Get<Mesh>(mAssetPipeline.GetManifest().GetUUID("sponza"));
    EventDispatcher::AddListener<MouseEvent>(std::bind_front(&Editor::OnMouseEvent, this));
    EventDispatcher::AddListener<KeyEvent>(std::bind_front(&Editor::OnKeyEvent, this));
}

void Editor::OnUpdate(f32 deltaTimeSeconds)
{
    mUIManager->Update(deltaTimeSeconds);

    if (Input::IsKeyDown(Key::W) || Input::IsKeyDown(Key::S) || Input::IsKeyDown(Key::A) || Input::IsKeyDown(Key::D))
    {
        Vec3f moveVector;
        moveVector += Input::IsKeyDown(Key::W) ? mCamera.Front() : Input::IsKeyDown(Key::S) ? -mCamera.Front() : Vec3f{};
        moveVector += Input::IsKeyDown(Key::A) ? -mCamera.Right() : Input::IsKeyDown(Key::D) ? mCamera.Right() : Vec3f{};
        moveVector.Normalize();
        mCamera.Translate(moveVector * deltaTimeSeconds * 15.0f);
    }
}

void Editor::OnWindowResize()
{
    mCamera.OnWindowResize(Config.WindowWidth, Config.WindowHeight);
    mUIManager->OnWindowResize(Config.WindowWidth, Config.WindowHeight);
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
            auto* root = mUIManager->GetRootWidget();
            root->SetVisible(!root->IsVisible());
        }
        else if (e.Key == Key::F3)
        {
            leftPanel->SetWidth(800.0f);
        }
        else if (e.Key == Key::F4)
        {
            leftPanel->SetWidth(300.0f);
        }
    }

    return true;
}