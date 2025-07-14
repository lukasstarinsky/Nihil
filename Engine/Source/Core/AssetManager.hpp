#pragma once

#include <filesystem>
#include "Graphics/Texture.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Mesh.hpp"

class NIHIL_API AssetManager
{
public:
    virtual ~AssetManager() = default;

    virtual auto LoadTexture(std::string_view name) const -> TextureSpecification = 0;
    virtual auto LoadShader(std::string_view name, ShaderStage shaderStage) const -> ShaderSpecification = 0;
    virtual auto LoadMesh(std::string_view file, std::string_view name) const -> MeshSpecification = 0;

    virtual void PackAll() const = 0;

    auto GetDefaultVertexShader() const -> ShaderPtr;
    auto GetDefaultFragmentShader() const -> ShaderPtr;
    auto GetDefaultMaterial() const -> MaterialPtr;
protected:
    AssetManager() = default;
protected:
    ShaderPtr mDefaultVertexShader {};
    ShaderPtr mDefaultFragmentShader {};
    MaterialPtr mDefaultMaterial {};
};

class NIHIL_API RawAssetManager : public AssetManager
{
public:
    explicit RawAssetManager(const std::filesystem::path& root);

    auto LoadTexture(std::string_view name) const -> TextureSpecification override;
    auto LoadShader(std::string_view name, ShaderStage shaderStage) const -> ShaderSpecification override;
    auto LoadMesh(std::string_view file, std::string_view name) const -> MeshSpecification override;

    void PackAll() const override;
private:
    std::filesystem::path mRoot {};
};

class PackedAssetManager : public AssetManager
{
public:
    explicit PackedAssetManager() = default;

    auto LoadTexture(std::string_view name) const -> TextureSpecification override;
    auto LoadShader(std::string_view name, ShaderStage shaderStage) const -> ShaderSpecification override;
    auto LoadMesh(std::string_view file, std::string_view name) const -> MeshSpecification override;

    void PackAll() const override;
};