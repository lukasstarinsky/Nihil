#include "RawAssetManager.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb/stb_image.h>
#include "Graphics/Renderer.hpp"
#include "ShaderCompiler.hpp"

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
        if (entry.is_directory())
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
        .Name = name.data(),
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
        if (entry.is_directory())
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
            shaderSpec.Name = std::string(name) + (api == RendererAPI::OpenGL ? ".glspv" : ".vkspv");
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

auto RawAssetManager::LoadMesh(std::string_view file, std::string_view name) const -> MeshSpecification
{
    auto meshSpecs = LoadAllMeshes(file);
    auto spec = std::find_if(meshSpecs.begin(), meshSpecs.end(), [&](const MeshSpecification& meshSpec) { return meshSpec.Name == std::format("{}::{}", file, name); });
    Ensure(spec != meshSpecs.end(), "Mesh '{}' not found in file '{}'.", name, file);
    return *spec;
}

auto RawAssetManager::LoadAllMeshes(std::string_view file) const -> std::vector<MeshSpecification>
{
    std::filesystem::path filePath {};
    for (const auto& entry: std::filesystem::recursive_directory_iterator(mRoot / "Models"))
    {
        if (entry.is_directory())
            continue;

        // File name without extension
        auto assetName = entry.path().filename().replace_extension().string();
        if (assetName == file)
        {
            filePath = entry.path();
            break;
        }
    }
    Ensure(!filePath.empty(), "Mesh file '{}' not found.", file);

    auto fileData = File::ReadBinary(filePath);
    Assimp::Importer importer;
    const auto* scene = importer.ReadFileFromMemory(fileData.data(), static_cast<u32>(fileData.size()), aiProcess_Triangulate);
    Ensure(scene && scene->HasMeshes(), "Failed to load mesh from file: {}", filePath.string());

    std::span<aiMesh*> loadedMeshes(scene->mMeshes, scene->mMeshes + scene->mNumMeshes);
    std::vector<MeshSpecification> meshSpecs;
    meshSpecs.reserve(loadedMeshes.size());
    for (const auto* mesh: loadedMeshes)
    {
        if (mesh->mName.length == 0)
        {
            Logger::Warn("Mesh with no name found in file '{}', skipping.", filePath.string());
            continue;
        }

        MeshSpecification meshSpec {};
        meshSpec.Name = std::format("{}::{}", file, mesh->mName.C_Str());

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

        meshSpecs.push_back(meshSpec);
    }

    return meshSpecs;
}

void RawAssetManager::PackAll(const std::filesystem::path& outFilePath) const
{
    for (const auto& entry: std::filesystem::recursive_directory_iterator(mRoot / "Textures"))
    {
        if (entry.is_directory())
            continue;

        auto assetName = entry.path().filename().replace_extension().string();
        auto textureSpec = RawAssetManager::LoadTexture(assetName);

        // TODO: Pack
        Logger::Info("Packing texture '{}'", textureSpec.Name);
    }

    for (const auto& entry: std::filesystem::recursive_directory_iterator(mRoot / "Shaders"))
    {
        if (entry.is_directory())
            continue;

        auto& path = entry.path();
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
        if (path.extension() == ".glsl")
        {
            for (i32 i = 0; i < 2; ++i)
            {
                auto binary = ShaderCompiler::GlslToSpv(sourceCode, shaderStage, i == 0 ? RendererAPI::OpenGL : RendererAPI::Vulkan);
                ShaderSpecification shaderSpec {
                    .Name = strippedExt + (i == 0 ? ".glspv" : ".vkspv"),
                    .Stage = shaderStage,
                    .Data = std::vector<std::byte>(binary.size() * sizeof(binary[0]))
                };
                std::memcpy(shaderSpec.Data.data(), binary.data(), shaderSpec.Data.size());
                Logger::Info("Packing shader '{}'", shaderSpec.Name);

                // Temporary break, vulkan is not supported yet
                break;
            }
        }
        else if (path.extension() == ".hlsl")
        {
            Logger::Info("Packing shader '{}' for Direct3D", strippedExt);
        }
        else if (path.extension() == ".msl")
        {
            Logger::Info("Packing shader '{}' for Metal", strippedExt);
        }

        // TODO: Pack
    }

    for (const auto& entry: std::filesystem::recursive_directory_iterator(mRoot / "Models"))
    {
        if (entry.is_directory())
            continue;

        auto assetName = entry.path().filename().replace_extension().string();
        auto meshSpecs = RawAssetManager::LoadAllMeshes(assetName);

        for (const auto& meshSpec: meshSpecs)
        {
            // TODO: Pack
            Logger::Info("Packing mesh '{}'", meshSpec.Name);
        }
    }
}
