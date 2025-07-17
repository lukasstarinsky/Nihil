#pragma once

#include <filesystem>
#include "AssetManager.hpp"

class NIHIL_API RawAssetManager : public AssetManager
{
public:
    explicit RawAssetManager(const std::filesystem::path& root);

    auto LoadTexture(std::string_view name) const -> TextureSpecification override;
    auto LoadShader(std::string_view name) const -> ShaderSpecification override;
    auto LoadMesh(std::string_view file, std::string_view name) const -> MeshSpecification override;

    void PackAll(const std::filesystem::path& outFilePath) const override;
private:
    auto LoadAllMeshes(std::string_view file) const -> std::vector<MeshSpecification>;
private:
    std::filesystem::path mRoot {};
};