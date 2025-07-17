#include "PackedAssetManager.hpp"

auto PackedAssetManager::LoadTexture(std::string_view name) const -> TextureSpecification
{
    Ensure(false, "PackedAssetManager::LoadTexture is not implemented yet.");
    return {};
}

auto PackedAssetManager::LoadShader(std::string_view name) const -> ShaderSpecification
{
    Ensure(false, "PackedAssetManager::LoadShader is not implemented yet.");
    return {};
}

auto PackedAssetManager::LoadMesh(std::string_view file, std::string_view name) const -> MeshSpecification
{
    Ensure(false, "PackedAssetManager::LoadMesh is not implemented yet.");
    return {};
}

void PackedAssetManager::PackAll([[maybe_unused]] const std::filesystem::path& outFilePath) const
{
    // Do nothing in already packed manager
}