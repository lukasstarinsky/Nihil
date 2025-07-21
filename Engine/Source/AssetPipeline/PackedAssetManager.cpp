#include "PackedAssetManager.hpp"

#include "Graphics/Renderer.hpp"

PackedAssetManager::PackedAssetManager(const std::filesystem::path& pakFile)
    : mPakReader{pakFile}
{
    mDefaultVertexShader = Shader::Create(PackedAssetManager::LoadShader("DefaultObjectShader.vs"));
    mDefaultFragmentShader = Shader::Create(PackedAssetManager::LoadShader("DefaultObjectShader.fs"));
    mDefaultMaterial = Material::Create(mDefaultVertexShader, mDefaultFragmentShader);
}

auto PackedAssetManager::LoadTexture(std::string_view name) const -> TextureSpecification
{
    return mPakReader.Read<TextureSpecification>(name);
}

auto PackedAssetManager::LoadShader(std::string_view name) const -> ShaderSpecification
{
    std::string fullName;
    switch (Renderer::GetApi())
    {
    case RendererAPI::OpenGL:
        fullName = std::format("{}{}", name, ".glspv");
        break;
    case RendererAPI::Vulkan:
        fullName = std::format("{}{}", name, ".vkspv");
        break;
    case RendererAPI::Direct3D11:
    case RendererAPI::Direct3D12:
    case RendererAPI::Metal:
        break;
    }

    auto spec = mPakReader.Read<ShaderSpecification>(fullName);
    if (name.ends_with(".vs"))
    {
        spec.Stage = ShaderStage::Vertex;
    }
    else if (name.ends_with(".fs"))
    {
        spec.Stage = ShaderStage::Fragment;
    }

    return spec;
}

auto PackedAssetManager::LoadMesh(std::string_view file, std::string_view name) const -> MeshSpecification
{
    auto fullName = std::format("{}::{}", file, name);
    return mPakReader.Read<MeshSpecification>(fullName);
}

void PackedAssetManager::PackAll(
    [[maybe_unused]] const std::filesystem::path& outFilePath,
    [[maybe_unused]] i32 compressionLevel,
    [[maybe_unused]] u32 compressionThreshold
) const
{
    // Do nothing in already packed manager
}