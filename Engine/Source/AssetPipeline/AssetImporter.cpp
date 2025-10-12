#include "AssetImporter.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb/stb_image.h>

#include "ShaderCompiler.hpp"

auto AssetImporter::ImportTexture(const std::filesystem::path& filePath) -> TextureSpecification
{
    auto fileData = File::ReadBinary(filePath);
    i32 width, height, numChannels;
    auto data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(fileData.data()), static_cast<int>(fileData.size()), &width, &height, &numChannels, STBI_rgb_alpha);
    Ensure(data, "Failed to load texture '{}'", filePath.filename().string());

    auto size = width * height * 4;

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
    else
    {
        Logger::Warn("Unsupported shader format: {}", extension.string());
    }
    return {};
}

auto AssetImporter::ImportMesh(const std::filesystem::path& filePath) -> ImportedMesh
{
    auto fileData = File::ReadBinary(filePath);
    u32 flags = aiProcess_Triangulate;
    if (filePath.extension() == ".obj")
    {
        flags |= aiProcess_FlipUVs;
    }

    Assimp::Importer importer;
    const auto* scene = importer.ReadFileFromMemory(fileData.data(), static_cast<u32>(fileData.size()), flags);
    Ensure(scene && scene->HasMeshes(), "Failed to load mesh from file: {}", filePath.string());
    std::span<aiMesh*> loadedMeshes(scene->mMeshes, scene->mMeshes + scene->mNumMeshes);

    ImportedMesh importedMesh {};
    importedMesh.SubMeshes.reserve(scene->mNumMeshes);

    for (const auto* mesh: loadedMeshes)
    {
        if (mesh->mName.length == 0)
        {
            Logger::Warn("Mesh with no name found in file '{}', skipping.", filePath.string());
            continue;
        }

        SubMesh subMesh {
            .MaterialIndex = mesh->mMaterialIndex,
            .BaseVertex = static_cast<u32>(importedMesh.Vertices.size()),
            .BaseIndex = static_cast<u32>(importedMesh.Indices.size()),
            .IndexCount = mesh->mNumFaces * 3
        };
        importedMesh.SubMeshes.push_back(subMesh);

        // Load vertices
        for (u32 i = 0; i < mesh->mNumVertices; ++i)
        {
            Vertex vertex {};
            vertex.Position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
            if (mesh->HasTextureCoords(0))
            {
                vertex.TexCoord = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
            }
            importedMesh.Vertices.push_back(vertex);
        }

        // Load indices
        for (u32 i = 0; i < mesh->mNumFaces; ++i)
        {
            const auto& face = mesh->mFaces[i];
            for (u32 j = 0; j < face.mNumIndices; ++j)
            {
                importedMesh.Indices.push_back(face.mIndices[j]);
            }
        }
    }

    if (filePath.extension() == ".obj")
    {
        // Process .mtl manually because assimp panics when textures are not in the same directory
        auto mtlPath = filePath;
        mtlPath.replace_extension(".mtl");

        std::unordered_map<std::string, u32> materialIndexMap;

        if (std::filesystem::exists(mtlPath))
        {
            auto mtl = File::ReadLines(mtlPath);

            for (const auto& line: mtl)
            {
                if (line.starts_with("newmtl "))
                {
                    auto name = line.substr(7);
                    materialIndexMap[name] = static_cast<u32>(importedMesh.Materials.size());

                    auto& material = importedMesh.Materials.emplace_back();
                    material.Name = name;
                }
                else if (line.starts_with("map_Kd "))
                {
                    auto& material = importedMesh.Materials.back();
                    auto texturePath = line.substr(7);
                    auto modelName = filePath.stem().string();
                    auto textureName = texturePath.substr(texturePath.find_last_of('/') + 1);
                    material.TextureNames.push_back(std::format("{}/{}", modelName, textureName));
                }
            }

            // Remap submesh material indices
            for (auto& subMesh: importedMesh.SubMeshes)
            {
                auto material = scene->mMaterials[subMesh.MaterialIndex];
                auto it = materialIndexMap.find(material->GetName().C_Str());
                if (it != materialIndexMap.end())
                {
                    subMesh.MaterialIndex = it->second;
                }
                else
                {
                    subMesh.MaterialIndex = 0;
                }
            }
        }
    }

    return importedMesh;
}