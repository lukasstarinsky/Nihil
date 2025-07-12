#include "AssetManager.hpp"

#include <stb/stb_image.h>
#include "Graphics/Renderer.hpp"
#include "Graphics/ShaderCompiler.hpp"

RawAssetManager::RawAssetManager(const std::filesystem::path& root)
    : mRoot{root}
{

}

auto RawAssetManager::LoadTexture(std::string_view name) const -> TextureSpecification
{
    auto filePath = mRoot / "Textures" / name;
    auto fileData = File::ReadBinary(filePath);

    i32 width, height, numChannels;
    auto data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(fileData.data()), static_cast<int>(fileData.size()), &width, &height, &numChannels, 4);
    Ensure(data, "Failed to load image: {}", filePath.string());

    auto size = width * height * numChannels;

    TextureSpecification textureSpec {
        .Width = width,
        .Height = height,
        .Data = std::vector<std::byte>(size)
    };
    std::memcpy(textureSpec.Data.data(), data, size);

    stbi_image_free(data);
    return textureSpec;
}

auto RawAssetManager::LoadShader(std::string_view name, ShaderStage shaderStage) const -> ShaderSpecification
{
    auto filePath = mRoot / "Shaders" / name;
    if (Renderer::GetApi() == RendererAPI::OpenGL || Renderer::GetApi() == RendererAPI::Vulkan)
    {
        filePath += ".glsl";

        auto sourceCode = File::Read(filePath);
        auto binary = ShaderCompiler::GlslToSpv(sourceCode, shaderStage);

        ShaderSpecification shaderSpec {
            .Stage = shaderStage,
            .Data = std::vector<std::byte>(binary.size() * sizeof(binary[0]))
        };
        std::memcpy(shaderSpec.Data.data(), binary.data(), shaderSpec.Data.size());
        return shaderSpec;
    }

    return {};
}