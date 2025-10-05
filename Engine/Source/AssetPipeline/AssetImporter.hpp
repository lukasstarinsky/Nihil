#pragma once

#include <filesystem>

#include "AssetManager.hpp"

class RawAssetManager : public AssetManager
{
public:
    explicit RawAssetManager(const std::filesystem::path& root);

    auto LoadTexture(std::string_view name) const -> TextureSpecification override;
    auto LoadShader(std::string_view name) const -> ShaderSpecification override;
    auto LoadMesh(std::string_view name) const -> MeshSpecification override;

    void PackAll(const std::filesystem::path& outFilePath, i32 compressionLevel, u32 compressionThreshold) const override;
private:
    std::filesystem::path mRoot {};
};