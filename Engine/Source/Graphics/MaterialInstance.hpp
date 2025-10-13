#pragma once

#include <unordered_map>

#include "Material.hpp"
#include "Resource.hpp"
#include "UniformBuffer.hpp"

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

    template <typename T>
    void SetData(const std::string& name, const T& data)
    {
        auto it = std::find_if(mBaseMaterial->mLayout.begin(), mBaseMaterial->mLayout.end(), [&name](const MaterialParameter& param) {
            return param.Name == name;
        });
        ASSERT(it != mBaseMaterial->mLayout.end(), "Material parameter not found: {}", name);
        std::memcpy(mUniformData.data() + it->Offset, &data, sizeof(T));
    }
    void UploadData() const;

    static auto Create(const MaterialInstanceCreateInfo& createInfo) -> MaterialInstancePtr;
private:
    MaterialPtr mBaseMaterial {};
    UniformBufferPtr mUniformBuffer {};
    std::vector<std::byte> mUniformData {};
    std::unordered_map<i32, TexturePtr> mTextures {};
};