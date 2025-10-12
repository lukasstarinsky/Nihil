#pragma once

#include <unordered_map>

#include "Material.hpp"
#include "Resource.hpp"
#include "Buffer.hpp"

struct MaterialInstanceSpecification
{
    Nihil::UUID UUID {};
    Nihil::UUID BaseMaterialUUID {};
    std::vector<std::byte> UniformData {};
    std::unordered_map<i32, Nihil::UUID> Textures {};
};

struct MaterialInstanceCreateInfo
{
    MaterialPtr BaseMaterial {};
    std::vector<std::byte> UniformData {};
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
//    void SetTexture(i32 slot, const TexturePtr& texture);
    void UploadData() const;

    static auto Create(const MaterialInstanceCreateInfo& createInfo) -> MaterialInstancePtr;
private:
    MaterialPtr mBaseMaterial {};
    BufferPtr mUniformBuffer {};
    std::vector<std::byte> mUniformData {};
    std::unordered_map<i32, TexturePtr> mTextures {};
};