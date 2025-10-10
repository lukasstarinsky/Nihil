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

    template <typename T> requires IsAnyOf<T, Texture, Shader, Mesh>
    auto Get(const Nihil::UUID& uuid) -> std::shared_ptr<T>
    {
        auto it = mCache.find(uuid);
        if (it != mCache.end())
        {
            return std::dynamic_pointer_cast<T>(it->second);
        }

        Ensure(HasAsset(uuid), "{} with UUID '{}' not found in asset pack.", typeid(T).name(), uuid);
        auto spec = mPakReader.Read<typename T::Specification>(uuid);
        std::shared_ptr<T> resource {};

        if constexpr (std::is_same_v<T, Mesh>)
        {
            MeshCreateInfo meshCreateInfo {
                .Vertices = std::move(spec.Vertices),
                .Indices = std::move(spec.Indices),
                .SubMeshes = std::move(spec.SubMeshes),
                .Materials = {}
            };
            meshCreateInfo.Materials.reserve(spec.Materials.size());

            for (const auto& matSpec: spec.Materials)
            {
                MaterialCreateInfo materialCreateInfo {
                    .VertexShader = AssetManager::Get<Shader>(matSpec.VertexShaderUUID),
                    .FragmentShader = AssetManager::Get<Shader>(matSpec.FragmentShaderUUID),
                    .Texture = AssetManager::Get<Texture>(matSpec.TextureUUID)
                };
                meshCreateInfo.Materials.push_back(Material::Create(materialCreateInfo));
            }
            resource = Mesh::Create(meshCreateInfo);
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