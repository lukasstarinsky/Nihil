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
    virtual auto LoadShader(std::string_view name) const -> ShaderSpecification = 0;
    virtual auto LoadMesh(std::string_view file, std::string_view name) const -> MeshSpecification = 0;

    virtual void PackAll(const std::filesystem::path& outFilePath) const = 0;

    auto GetDefaultVertexShader() const -> ShaderPtr { return mDefaultVertexShader; };
    auto GetDefaultFragmentShader() const -> ShaderPtr { return mDefaultFragmentShader; };
    auto GetDefaultMaterial() const -> MaterialPtr { return mDefaultMaterial; };
protected:
    AssetManager() = default;
protected:
    ShaderPtr mDefaultVertexShader {};
    ShaderPtr mDefaultFragmentShader {};
    MaterialPtr mDefaultMaterial {};
};