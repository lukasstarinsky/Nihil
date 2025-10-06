#include "AssetImporter.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb/stb_image.h>

#include "Graphics/Renderer.hpp"
#include "ShaderCompiler.hpp"

auto AssetImporter::ImportTexture(const std::filesystem::path& filePath) -> TextureSpecification
{
    auto fileData = File::ReadBinary(filePath);
    i32 width, height, numChannels;
    auto data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(fileData.data()), static_cast<int>(fileData.size()), &width, &height, &numChannels, 4);
    Ensure(data, "Failed to load texture '{}'", filePath.filename().string());

    auto size = width * height * numChannels;

    TextureSpecification textureSpec {
        .UUID = Nihil::UUID::Generate(),
        .Width = width,
        .Height = height,
        .Data = std::vector<std::byte>(size)
    };
    std::memcpy(textureSpec.Data.data(), data, size);

    stbi_image_free(data);
    return textureSpec;
}

auto AssetImporter::ImportShader(const std::filesystem::path& path) -> ShaderSpecification
{
    auto extension = path.extension();
    auto strippedExt = path.filename().replace_extension().string();

    ShaderStage shaderStage {};
    if (strippedExt.ends_with(".vs"))
    {
        shaderStage = ShaderStage::Vertex;
    }
    else if (strippedExt.ends_with(".fs"))
    {
        shaderStage = ShaderStage::Fragment;
    }

    // TODO: Compile every shader for every supported backend
    auto sourceCode = File::Read(path);
    if (extension == ".glsl")
    {
//        auto vkBinary = ShaderCompiler::GlslToSpv(sourceCode, shaderStage, RendererAPI::Vulkan);
        auto glBinary = ShaderCompiler::GlslToSpv(sourceCode, shaderStage, RendererAPI::OpenGL);

        ShaderSpecification shaderSpec {};
        shaderSpec.UUID = Nihil::UUID::Generate();
        shaderSpec.Stage = shaderStage;
        shaderSpec.Variants = {
//                ShaderVariant{ RendererAPI::Vulkan, std::vector<std::byte>(vkBinary.size() * sizeof(vkBinary[0])) },
            ShaderVariant{ RendererAPI::OpenGL, std::vector<std::byte>(glBinary.size() * sizeof(glBinary[0])) }
        };
//        std::memcpy(shaderSpec.Variants[0].Data.data(), vkBinary.data(), shaderSpec.Variants[0].Data.size());
        std::memcpy(shaderSpec.Variants[0].Data.data(), glBinary.data(), shaderSpec.Variants[0].Data.size());
        return shaderSpec;
    }
    else if (extension == ".hlsl")
    {
        Logger::Info("Packing shader '{}' for Direct3D", strippedExt);
    }
    else if (extension == ".msl")
    {
        Logger::Info("Packing shader '{}' for Metal", strippedExt);
    }
    return {};
}

auto AssetImporter::ImportMesh(const std::filesystem::path& filePath) -> MeshSpecification
{
    auto fileData = File::ReadBinary(filePath);
    Assimp::Importer importer;
    const auto* scene = importer.ReadFileFromMemory(fileData.data(), static_cast<u32>(fileData.size()), aiProcess_Triangulate);
    Ensure(scene && scene->HasMeshes(), "Failed to load mesh from file: {}", filePath.string());

    std::span<aiMesh*> loadedMeshes(scene->mMeshes, scene->mMeshes + scene->mNumMeshes);
    MeshSpecification meshSpec {};
    meshSpec.UUID = Nihil::UUID::Generate();
    meshSpec.SubMeshes.reserve(scene->mNumMeshes);

    for (const auto* mesh: loadedMeshes)
    {
        if (mesh->mName.length == 0)
        {
            Logger::Warn("Mesh with no name found in file '{}', skipping.", filePath.string());
            continue;
        }

        SubMesh subMesh {
            .BaseVertex = static_cast<u32>(meshSpec.Vertices.size()),
            .BaseIndex = static_cast<u32>(meshSpec.Indices.size()),
            .IndexCount = mesh->mNumFaces * 3
        };
        meshSpec.SubMeshes.push_back(subMesh);

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
    }

    return meshSpec;
}