#include "Panel.hpp"

#include "Graphics/Renderer.hpp"

namespace UI
{

Panel::Panel(Widget* parent)
    : Widget{parent}
{
}

void Panel::OnInitialize()
{
    mMaterial = MaterialInstance::Create({
        .BaseMaterial = mUIBaseMaterial,
        .UniformData = {},
        .Textures = {}
    });
    mMaterial->SetData("uBaseColor", Vec4f{0.2f, 0.2f, 0.8f, 1.0f});
    mMaterial->UploadData();
}

void Panel::Render() const
{
    Renderer::Draw(mQuadMesh, mMaterial, Mat4f::Translate({200.0f, 200.0f, 0.0f}) * Mat4f::Scale({100.0f, 100.0f, 1.0f}));
    Widget::Render();
}

}