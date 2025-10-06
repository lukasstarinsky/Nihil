#include "AssetManager.hpp"

#include "Graphics/Renderer.hpp"

AssetManager::AssetManager(const std::filesystem::path& pakFile)
    : mPakReader{pakFile}
{
}

auto AssetManager::HasAsset(const Nihil::UUID& uuid) const -> bool
{
    return mPakReader.HasEntry(uuid);
}

auto AssetManager::GetTexture(const Nihil::UUID& uuid) const -> TextureSpecification
{
    Ensure(HasAsset(uuid), "Texture with UUID '{}' not found in asset pack.", uuid);
    return mPakReader.Read<TextureSpecification>(uuid);
}

auto AssetManager::GetShader(const Nihil::UUID& uuid) const -> ShaderSpecification
{
    Ensure(HasAsset(uuid), "Shader with UUID '{}' not found in asset pack.", uuid);
    return mPakReader.Read<ShaderSpecification>(uuid);
}

auto AssetManager::GetMesh(const Nihil::UUID& uuid) const -> MeshSpecification
{
    Ensure(HasAsset(uuid), "Mesh with UUID '{}' not found in asset pack.", uuid);
    return mPakReader.Read<MeshSpecification>(uuid);
}