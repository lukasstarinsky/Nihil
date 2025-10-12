#pragma once

#include "PakReader.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Resource.hpp"

class AssetManager
{
public:
    explicit AssetManager(const std::filesystem::path& pakFile);

    auto HasAsset(const Nihil::UUID& uuid) const -> bool;

    template <typename T> requires IsAnyOf<T, Texture, Shader, Mesh, Material, MaterialInstance>
    auto Get(const Nihil::UUID& uuid) -> std::shared_ptr<T>
    {
        auto it = mCache.find(uuid);
        if (it != mCache.end())
        {
            return std::dynamic_pointer_cast<T>(it->second);
        }

        Ensure(HasAsset(uuid), "{} with UUID '{}' not found in asset pack.", typeid(T).name(), uuid);
        auto spec = mPakReader.Deserialize<typename T::Specification>(uuid);
        std::shared_ptr<T> resource {};

        if constexpr (std::is_same_v<T, Mesh>)
        {
            MeshCreateInfo createInfo {
                .Vertices = std::move(spec.Vertices),
                .Indices = std::move(spec.Indices),
                .SubMeshes = std::move(spec.SubMeshes),
                .Materials = {}
            };
            createInfo.Materials.reserve(spec.Materials.size());

            for (const auto& matUUID: spec.Materials)
            {
                auto material = AssetManager::Get<MaterialInstance>(matUUID);
                createInfo.Materials.push_back(material);
            }
            resource = T::Create(createInfo);
        }
        else if constexpr (std::is_same_v<T, Material>)
        {
            MaterialCreateInfo createInfo {
                .VertexShader = AssetManager::Get<Shader>(spec.VertexShaderUUID),
                .FragmentShader = AssetManager::Get<Shader>(spec.FragmentShaderUUID),
                .Layout = std::move(spec.Layout)
            };
            resource = T::Create(createInfo);
        }
        else if constexpr (std::is_same_v<T, MaterialInstance>)
        {
            auto baseMaterial = AssetManager::Get<Material>(spec.BaseMaterialUUID);
            MaterialInstanceCreateInfo createInfo {
                .BaseMaterial = baseMaterial,
                .UniformData = std::move(spec.UniformData),
                .Textures = {}
            };
            for (const auto& [slot, texUUID] : spec.Textures)
            {
                auto texture = AssetManager::Get<Texture>(texUUID);
                createInfo.Textures[slot] = texture;
            }
            resource = T::Create(createInfo);
        }
        else
        {
            resource = T::Create(spec);
        }

        if (!resource)
        {
            Logger::Error("Failed to load {} with UUID '{}'", typeid(T).name(), uuid);
        }
        mCache[uuid] = resource;
        return resource;
    }
private:
    PakReader mPakReader;
    std::unordered_map<Nihil::UUID, std::shared_ptr<Resource>> mCache;
};