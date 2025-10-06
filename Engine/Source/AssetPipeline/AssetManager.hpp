#pragma once

#include "PakReader.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Material.hpp"

class AssetManager
{
public:
    explicit AssetManager(const std::filesystem::path& pakFile);

    auto HasAsset(const Nihil::UUID& uuid) const -> bool;

    auto GetTexture(const Nihil::UUID& uuid) const -> TextureSpecification;
    auto GetShader(const Nihil::UUID& uuid) const -> ShaderSpecification;
    auto GetMesh(const Nihil::UUID& uuid) const -> MeshSpecification;
private:
    PakReader mPakReader;
};