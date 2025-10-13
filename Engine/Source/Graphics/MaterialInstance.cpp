#include "MaterialInstance.hpp"

MaterialInstance::MaterialInstance(const MaterialInstanceCreateInfo& createInfo)
    : mBaseMaterial{createInfo.BaseMaterial}
    , mUniformData{createInfo.UniformData}
    , mTextures{createInfo.Textures}
{
    ASSERT(!mBaseMaterial->mLayout.empty());

    UniformBufferCreateInfo bufferCreateInfo {
        .Data = nullptr,
        .Size = mBaseMaterial->mLayoutSize,
        .UniformBinding = UniformBinding::Material
    };
    mUniformBuffer = UniformBuffer::Create(bufferCreateInfo);

    if (static_cast<i32>(mUniformData.size()) != mBaseMaterial->mLayoutSize)
    {
        mUniformData.resize(mBaseMaterial->mLayoutSize);
        UploadData();
    }
}

void MaterialInstance::Bind() const
{
    mBaseMaterial->Bind();
    mUniformBuffer->Bind();
    for (const auto& [slot, texture]: mTextures)
    {
        texture->Bind(slot);
    }
}

void MaterialInstance::UploadData() const
{
    mUniformBuffer->SetData(mUniformData.data(), static_cast<i32>(mUniformData.size()), 0);
}

auto MaterialInstance::Create(const MaterialInstanceCreateInfo& createInfo) -> MaterialInstancePtr
{
    return std::make_shared<MaterialInstance>(createInfo);
}