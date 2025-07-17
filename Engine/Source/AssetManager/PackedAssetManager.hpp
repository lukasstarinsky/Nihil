#pragma once

#include "AssetManager.hpp"
#include "PakFile.hpp"

class NIHIL_API PackedAssetManager : public AssetManager
{
public:
    explicit PackedAssetManager(const std::filesystem::path& pakFile);

    auto LoadTexture(std::string_view name) const -> TextureSpecification override;
    auto LoadShader(std::string_view name) const -> ShaderSpecification override;
    auto LoadMesh(std::string_view file, std::string_view name) const -> MeshSpecification override;

    void PackAll(const std::filesystem::path& outFilePath) const override;
private:
    PakReader mPakReader;
};
