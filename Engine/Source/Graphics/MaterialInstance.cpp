#include "MaterialInstance.hpp"

MaterialInstance::MaterialInstance(const MaterialInstanceCreateInfo& createInfo)
    : mBaseMaterial{createInfo.BaseMaterial}
    , mTextures{createInfo.Textures}
{
}

void MaterialInstance::Bind() const
{
    mBaseMaterial->Bind();
    for (const auto& [slot, texture] : mTextures)
    {
        texture->Bind(slot);
    }
}

void MaterialInstance::SetUniform(i32 location, const Mat4f& data) const
{
    mBaseMaterial->SetUniform(location, data);
}

auto MaterialInstance::Create(const MaterialInstanceCreateInfo& createInfo) -> MaterialInstancePtr
{
    return std::make_shared<MaterialInstance>(createInfo);
}