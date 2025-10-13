#include "Manager.hpp"

#include "Graphics/Renderer.hpp"

namespace UI
{

Manager::Manager(AssetManager* assetManager)
    : mAssetManager{assetManager}
    , mCamera{CameraProjection::Orthographic, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 1920.0f, 1080.0f}
{
}

auto Manager::GetCamera() const -> const Camera&
{
    return mCamera;
}

void Manager::SetRootWidget(Widget* root)
{
    mRootWidget = root;
    root->SetAssetManager(mAssetManager);
}

void Manager::OnResize(i32 width, i32 height)
{
    mCamera.OnResize(width, height);
}

void Manager::Render() const
{
    mRootWidget->Render();
}

Manager::~Manager()
{
    delete mRootWidget;
}

}