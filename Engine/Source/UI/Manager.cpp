#include "Manager.hpp"

#include "Graphics/Renderer.hpp"

#define MAX_WIDGETS 128

namespace UI
{

Manager::Manager(AssetManager* assetManager, Widget* root)
    : mAssetManager{assetManager}
    , mRootWidget{root}
    , mCamera{CameraProjection::Orthographic, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 1280.0f, 960.0f}
{
    MaterialInstanceCreateInfo materialInstanceCreateInfo {
        .BaseMaterial = mAssetManager->Get<Material>(DefaultResource::UIMaterial),
        .UniformData = {},
        .Textures = {}
    };
    auto materialInstance = MaterialInstance::Create(materialInstanceCreateInfo);

    MeshCreateInfo uiMeshCreateInfo {
        .Vertices = {
            {.Position = {0.0f, 0.0f, 0.0f}, .TexCoord = {0.0f, 0.0f}},
            {.Position = {1.0f, 0.0f, 0.0f}, .TexCoord = {1.0f, 0.0f}},
            {.Position = {1.0f, 1.0f, 0.0f}, .TexCoord = {1.0f, 1.0f}},
            {.Position = {0.0f, 1.0f, 0.0f}, .TexCoord = {0.0f, 1.0f}},
        },
        .Indices = {
            0, 1, 2,
            0, 2, 3
        },
        .VertexLayout = VertexLayout::GetDefault(),
        .SubMeshes = {
            {.MaterialIndex = 0, .BaseVertex = 0, .BaseIndex = 0, .IndexCount = 6}
        },
        .Materials = { materialInstance }
    };
    mQuadMesh = Mesh::Create(uiMeshCreateInfo);

    BufferCreateInfo instanceBufferCreateInfo {
        .Flags = BufferFlags::DynamicStorage,
        .Data = nullptr,
        .Size = sizeof(WidgetInstanceData) * MAX_WIDGETS,
    };
    mQuadInstanceBuffer = Buffer::Create(instanceBufferCreateInfo);

    mInstanceVertexLayout = {
        .Stride = sizeof(WidgetInstanceData),
        .Attributes = {
            { 2, VertexAttribute::Type::Float2, offsetof(WidgetInstanceData, Position), 1, true },
            { 3, VertexAttribute::Type::Float2, offsetof(WidgetInstanceData, Size), 1, true }
        }
    };

    mQuadMesh->SetVertexBuffer(mQuadInstanceBuffer.get(), mInstanceVertexLayout, 1);

    EventDispatcher::AddListener<MouseEvent>(std::bind_front(&Manager::OnMouseEvent, this));
}

Manager::~Manager()
{
    delete mRootWidget;
}

auto Manager::GetCamera() const -> const Camera&
{
    return mCamera;
}

void Manager::OnResize(i32 width, i32 height)
{
    mCamera.OnResize(width, height);
}

void Manager::Render() const
{
    std::vector<WidgetInstanceData> instanceDatas;
    instanceDatas.reserve(MAX_WIDGETS);
    mRootWidget->CollectInstanceData(instanceDatas);

    // TODO: Sort by WidgetInstanceData::Layer
    ASSERT(instanceDatas.size() <= MAX_WIDGETS, "Max supported widgets: {}, current widgets: {}", MAX_WIDGETS, instanceDatas.size());

    mQuadMesh->GetMaterial(0)->Bind();
    mQuadInstanceBuffer->SetData(instanceDatas.data(), static_cast<i32>(sizeof(WidgetInstanceData) * instanceDatas.size()), 0);
    Renderer::DrawInstanced(mQuadMesh, static_cast<i32>(instanceDatas.size()));
}

auto Manager::OnMouseEvent(const MouseEvent& event) const -> bool
{
    if (event.Type != EventType::MousePress)
        return false;

    const auto* target = mRootWidget->GetLastWidgetAt(event.Position);
    if (target)
        return target->OnMouseClick();
    return false;
}

}