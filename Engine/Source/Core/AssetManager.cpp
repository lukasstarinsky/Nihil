#include "AssetManager.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb/stb_image.h>
#include "Graphics/Renderer.hpp"
#include "Graphics/ShaderCompiler.hpp"

auto AssetManager::GetDefaultVertexShader() const -> ShaderPtr
{
    return mDefaultVertexShader;
}

auto AssetManager::GetDefaultFragmentShader() const -> ShaderPtr
{
    return mDefaultFragmentShader;
}

auto AssetManager::GetDefaultMaterial() const -> MaterialPtr
{
    return mDefaultMaterial;
}

#pragma region RawAssetManager
RawAssetManager::RawAssetManager(const std::filesystem::path& root)
    : mRoot{root}
{
    ASSERT(std::filesystem::exists(mRoot), "Asset root directory does not exist: {}", mRoot.string());

    mDefaultVertexShader = Shader::Create(RawAssetManager::LoadShader("DefaultObjectShader.vert", ShaderStage::Vertex));
    mDefaultFragmentShader = Shader::Create(RawAssetManager::LoadShader("DefaultObjectShader.frag", ShaderStage::Fragment));
    mDefaultMaterial = Material::Create(mDefaultVertexShader, mDefaultFragmentShader);
}

auto RawAssetManager::LoadTexture(std::string_view name) const -> TextureSpecification
{
    auto validTextureExtensions = {".png", ".jpg", ".jpeg"};
    std::filesystem::path filePath {};

    for (const char* validExt : validTextureExtensions)
    {
        auto fullPath = mRoot / "Textures" / name;
        fullPath += validExt;

        if (std::filesystem::exists(fullPath))
        {
            filePath = fullPath;
            break;
        }
    }
    Ensure(!filePath.empty(), "Texture '{}' not found.", name);

    auto fileData = File::ReadBinary(filePath);
    i32 width, height, numChannels;
    auto data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(fileData.data()), static_cast<int>(fileData.size()), &width, &height, &numChannels, 4);
    Ensure(data, "Failed to load texture '{}'", name);

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

    switch(Renderer::GetApi())
    {
        case RendererAPI::OpenGL:
        case RendererAPI::Vulkan:
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
        case RendererAPI::Direct3D11:
        case RendererAPI::Direct3D12:
        case RendererAPI::Metal:
            Throw("Renderer API {} not yet supported.", Renderer::GetApiString());
    }

    return {};
}

auto RawAssetManager::LoadMesh(std::string_view file, std::string_view name) const -> MeshSpecification
{
    auto validMeshExtensions = {".obj", ".fbx"};
    std::filesystem::path filePath {};

    for (const char* validExt : validMeshExtensions)
    {
        auto fullPath = mRoot / "Models" / file;
        fullPath += validExt;

        if (std::filesystem::exists(fullPath))
        {
            filePath = fullPath;
            break;
        }
    }
    Ensure(!filePath.empty(), "Mesh file '{}' not found.", file);

    auto fileData = File::ReadBinary(filePath);
    Assimp::Importer importer;
    const auto* scene = importer.ReadFileFromMemory(fileData.data(), static_cast<u32>(fileData.size()), aiProcess_Triangulate);
    Ensure(scene && scene->HasMeshes(), "Failed to load mesh from file: {}", filePath.string());

    std::span<aiMesh*> loadedMeshes(scene->mMeshes, scene->mMeshes + scene->mNumMeshes);
    for (const auto* mesh: loadedMeshes)
    {
        // TODO: Load every mesh in the file, not just the first one with the matching name
        // Maybe use class Model with vector of meshes?
        if (mesh->mName.C_Str() != name)
            continue;

        MeshSpecification meshSpec {};
        meshSpec.Name = name;

        // Load vertices
        for (u32 i = 0; i < mesh->mNumVertices; ++i)
        {
            Vertex vertex {};
            vertex.Position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
            if (mesh->HasTextureCoords(0))
            {
                vertex.TexCoord = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
            }
            meshSpec.Vertices.push_back(vertex);
        }

        // Load indices
        for (u32 i = 0; i < mesh->mNumFaces; ++i)
        {
            const auto& face = mesh->mFaces[i];
            for (u32 j = 0; j < face.mNumIndices; ++j)
            {
                meshSpec.Indices.push_back(face.mIndices[j]);
            }
        }

        return meshSpec;
    }

    Throw("Mesh '{}' not found in file '{}'", name, filePath.string());
    return {};
}

void RawAssetManager::PackAll() const
{
    // TODO: Implement packing of assets
    // TODO: Recursively search the asset directory and pack all assets into a single file.
    Ensure(false, "RawAssetManager::PackAll is not implemented yet.");

}
#pragma endregion

#pragma region PackagedAssetManager
auto PackedAssetManager::LoadTexture(std::string_view name) const -> TextureSpecification
{
    Ensure(false, "PackedAssetManager::LoadTexture is not implemented yet.");
    return {};
}

auto PackedAssetManager::LoadShader(std::string_view name, ShaderStage shaderStage) const -> ShaderSpecification
{
    Ensure(false, "PackedAssetManager::LoadShader is not implemented yet.");
    return {};
}

auto PackedAssetManager::LoadMesh(std::string_view file, std::string_view name) const -> MeshSpecification
{
    Ensure(false, "PackedAssetManager::LoadMesh is not implemented yet.");
    return {};
}

void PackedAssetManager::PackAll() const
{
    // Do nothing in already packed manager
}
#pragma endregion