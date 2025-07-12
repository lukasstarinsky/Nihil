#pragma once

#include <filesystem>
#include "Graphics/Texture.hpp"
#include "Graphics/Shader.hpp"

class AssetManager
{
public:
    virtual ~AssetManager() = default;

    virtual auto LoadTexture(std::string_view name) const -> TextureSpecification = 0;
    virtual auto LoadShader(std::string_view name, ShaderStage shaderStage) const -> ShaderSpecification = 0;
protected:
    AssetManager() = default;
};

class NIHIL_API RawAssetManager : public AssetManager
{
public:
    RawAssetManager(const std::filesystem::path& root);

    auto LoadTexture(std::string_view name) const -> TextureSpecification override;
    auto LoadShader(std::string_view name, ShaderStage shaderStage) const -> ShaderSpecification override;
private:
    std::filesystem::path mRoot {};
};

class PackedAssetManager : public AssetManager
{
public:

};