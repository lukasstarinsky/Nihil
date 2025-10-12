#pragma once

#include <unordered_map>

#include "Material.hpp"
#include "Resource.hpp"

struct MaterialInstanceSpecification
{
    Nihil::UUID UUID {};
    Nihil::UUID BaseMaterialUUID {};
    std::unordered_map<i32, Nihil::UUID> Textures {};
};

struct MaterialInstanceCreateInfo
{
    MaterialPtr BaseMaterial {};
    std::unordered_map<i32, TexturePtr> Textures {};
};

class MaterialInstance;
using MaterialInstancePtr = std::shared_ptr<MaterialInstance>;

class MaterialInstance : public Resource
{
public:
    using Specification = MaterialInstanceSpecification;

    explicit MaterialInstance(const MaterialInstanceCreateInfo& createInfo);

    void Bind() const;
    void SetUniform(i32 location, const Mat4f& data) const;
//    void SetTexture(i32 slot, const TexturePtr& texture);

    static auto Create(const MaterialInstanceCreateInfo& createInfo) -> MaterialInstancePtr;
private:
    MaterialPtr mBaseMaterial {};
    std::unordered_map<i32, TexturePtr> mTextures {};
};