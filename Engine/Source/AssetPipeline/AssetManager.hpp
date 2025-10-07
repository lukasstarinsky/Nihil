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
            return std::static_pointer_cast<T>(it->second);
        }

        Ensure(HasAsset(uuid), "{} with UUID '{}' not found in asset pack.", typeid(T).name(), uuid);
        auto spec = mPakReader.Read<typename T::Specification>(uuid);
        auto resource = T::Create(spec);
        mCache[uuid] = resource;
        return resource;
    }
private:
    PakReader mPakReader;
    std::unordered_map<Nihil::UUID, std::shared_ptr<Resource>> mCache;
};