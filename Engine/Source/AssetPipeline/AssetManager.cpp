#include "AssetManager.hpp"

AssetManager::AssetManager(const std::filesystem::path& pakFile)
    : mPakReader{pakFile}
{
}

auto AssetManager::HasAsset(const Nihil::UUID& uuid) const -> bool
{
    return mPakReader.HasEntry(uuid);
}