#include "RawAssetManager.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb/stb_image.h>

#include "Graphics/Renderer.hpp"
#include "ShaderCompiler.hpp"
#include "PakWriter.hpp"

RawAssetManager::RawAssetManager(const std::filesystem::path& root)
    : mRoot{root}
{
    ASSERT(std::filesystem::exists(mRoot), "Asset root directory does not exist: {}", mRoot.string());

    mDefaultVertexShader = Shader::Create(RawAssetManager::LoadShader("DefaultObjectShader.vs"));
    mDefaultFragmentShader = Shader::Create(RawAssetManager::LoadShader("DefaultObjectShader.fs"));
    mDefaultMaterial = Material::Create(mDefaultVertexShader, mDefaultFragmentShader);
}

auto RawAssetManager::LoadTexture(std::string_view name) const -> TextureSpecification
{
    std::filesystem::path filePath {};
    for (const auto& entry: std::filesystem::recursive_directory_iterator(mRoot / "Textures"))
    {
        if (!entry.is_regular_file())
            continue;

        // File name without extension
        auto assetName = entry.path().filename().replace_extension().string();
        if (assetName == name)
        {
            filePath = entry.path();
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

auto RawAssetManager::LoadShader(std::string_view name) const -> ShaderSpecification
{
    auto api = Renderer::GetApi();

    std::string shaderExtension;
    switch (api)
    {
        case RendererAPI::OpenGL:
        case RendererAPI::Vulkan:
            shaderExtension = ".glsl";
            break;
        case RendererAPI::Direct3D11:
        case RendererAPI::Direct3D12:
            shaderExtension = ".hlsl";
            break;
        case RendererAPI::Metal:
            shaderExtension = ".msl";
            break;
    }

    ShaderStage shaderStage {};
    if (name.ends_with(".vs"))
    {
        shaderStage = ShaderStage::Vertex;
    }
    else if (name.ends_with(".fs"))
    {
        shaderStage = ShaderStage::Fragment;
    }

    std::filesystem::path filePath;
    for (const auto& entry: std::filesystem::recursive_directory_iterator(mRoot / "Shaders"))
    {
        if (!entry.is_regular_file())
            continue;

        auto assetName = entry.path().filename().replace_extension().string();
        if (assetName == name && entry.path().extension() == shaderExtension)
        {
            filePath = entry.path();
            break;
        }
    }
    Ensure(!filePath.empty(), "Shader '{}' not found.", name);

    ShaderSpecification shaderSpec {};
    shaderSpec.Stage = shaderStage;
    switch (api)
    {
        case RendererAPI::OpenGL:
        case RendererAPI::Vulkan:
        {
            auto sourceCode = File::Read(filePath);
            auto binary = ShaderCompiler::GlslToSpv(sourceCode, shaderSpec.Stage);
            shaderSpec.Data = std::vector<std::byte>(binary.size() * sizeof(binary[0]));
            shaderSpec.Name = std::format("{}{}", name, api == RendererAPI::OpenGL ? ".glspv" : ".vkspv");
            std::memcpy(shaderSpec.Data.data(), binary.data(), shaderSpec.Data.size());
            break;
        }
        case RendererAPI::Direct3D11:
        case RendererAPI::Direct3D12:
        case RendererAPI::Metal:
            Throw("Renderer API {} not yet supported.", Renderer::GetApiString());
    }

    return shaderSpec;
}

auto RawAssetManager::LoadMesh(std::string_view name) const -> MeshSpecification
{
    std::filesystem::path filePath {};
    for (const auto& entry: std::filesystem::recursive_directory_iterator(mRoot))
    {
        if (!entry.is_regular_file())
            continue;

        // File name without extension
        auto assetName = entry.path().filename().replace_extension().string();
        if (assetName == name)
        {
            filePath = entry.path();
            break;
        }
    }
    Ensure(!filePath.empty(), "Mesh file '{}' not found.", name);

    auto fileData = File::ReadBinary(filePath);
    Assimp::Importer importer;
    const auto* scene = importer.ReadFileFromMemory(fileData.data(), static_cast<u32>(fileData.size()), aiProcess_Triangulate);
    Ensure(scene && scene->HasMeshes(), "Failed to load mesh from file: {}", filePath.string());

    std::span<aiMesh*> loadedMeshes(scene->mMeshes, scene->mMeshes + scene->mNumMeshes);
    MeshSpecification meshSpec {};
    meshSpec.Name = name;
    meshSpec.SubMeshes.reserve(scene->mNumMeshes);

    for (const auto* mesh: loadedMeshes)
    {
        if (mesh->mName.length == 0)
        {
            Logger::Warn("Mesh with no name found in file '{}', skipping.", filePath.string());
            continue;
        }

        SubMesh subMesh {
            .Name = mesh->mName.C_Str(),
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

void RawAssetManager::PackAll(const std::filesystem::path& outFilePath, i32 compressionLevel, u32 compressionThreshold) const
{
    static const std::vector<const char*> textureExts { ".png", ".jpg", ".jpeg", ".bmp", ".tga" };
    static const std::vector<const char*> shaderExts { ".glsl", ".hlsl", ".msl" };
    static const std::vector<const char*> modelExts { ".obj", ".fbx", ".gltf", ".glb", ".dae" };

    PakWriter pakWriter(mRoot / outFilePath, compressionLevel, compressionThreshold);

    for (const auto& entry: std::filesystem::recursive_directory_iterator(mRoot))
    {
        if (!entry.is_regular_file())
            continue;

        auto& path = entry.path();
        auto extension = path.extension();

        // Pack textures
        if (std::find(textureExts.begin(), textureExts.end(), extension) != textureExts.end())
        {
            auto assetName = entry.path().filename().replace_extension().string();
            auto textureSpec = RawAssetManager::LoadTexture(assetName);

            Logger::Info("Packing texture '{}'", textureSpec.Name);
            pakWriter.Write(textureSpec);
        }
        // Pack shaders
        else if (std::find(shaderExts.begin(), shaderExts.end(), extension) != shaderExts.end())
        {
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

            auto sourceCode = File::Read(path);
            if (extension == ".glsl")
            {
                for (i32 i = 0; i < 2; ++i)
                {
                    auto binary = ShaderCompiler::GlslToSpv(sourceCode, shaderStage, i == 0 ? RendererAPI::OpenGL : RendererAPI::Vulkan);
                    ShaderSpecification shaderSpec {
                        .Name = std::format("{}{}", strippedExt, i == 0 ? ".glspv" : ".vkspv"),
                        .Stage = shaderStage,
                        .Data = std::vector<std::byte>(binary.size() * sizeof(binary[0]))
                    };
                    std::memcpy(shaderSpec.Data.data(), binary.data(), shaderSpec.Data.size());

                    Logger::Info("Packing shader '{}' for {}", shaderSpec.Name, i == 0 ? "OpenGL" : "Vulkan");
                    pakWriter.Write(shaderSpec);
                    break;
                }
            }
            else if (extension == ".hlsl")
            {
                Logger::Info("Packing shader '{}' for Direct3D", strippedExt);
            }
            else if (extension == ".msl")
            {
                Logger::Info("Packing shader '{}' for Metal", strippedExt);
            }
        }
        // Pack models
        else if (std::find(modelExts.begin(), modelExts.end(), extension) != modelExts.end())
        {
            auto assetName = entry.path().filename().replace_extension().string();
            auto meshSpec = RawAssetManager::LoadMesh(assetName);

            Logger::Info("Packing mesh '{}'", meshSpec.Name);
            pakWriter.Write(meshSpec);
        }
        else
        {
            Logger::Warn("Unsupported asset for packing: {}", path.string());
        }
    }

    pakWriter.Save();
}
