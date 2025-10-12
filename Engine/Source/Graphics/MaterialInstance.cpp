#include "MaterialInstance.hpp"

MaterialInstance::MaterialInstance(const MaterialInstanceCreateInfo& createInfo)
    : mBaseMaterial{createInfo.BaseMaterial}
    , mUniformData{createInfo.UniformData}
    , mTextures{createInfo.Textures}
{
    ASSERT(!mBaseMaterial->mLayout.empty());

    i32 bufferSize = 0;
    for (const auto& param: mBaseMaterial->mLayout)
    {
        bufferSize += static_cast<i32>(param.Type);
    }

    BufferCreateInfo bufferCreateInfo {
        .Type = BufferType::Uniform,
        .Data = nullptr,
        .Size = bufferSize,
        .UniformBinding = UniformBinding::Material
    };
    mUniformBuffer = Buffer::Create(bufferCreateInfo);
}

void MaterialInstance::Bind() const
{
    mBaseMaterial->Bind();
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