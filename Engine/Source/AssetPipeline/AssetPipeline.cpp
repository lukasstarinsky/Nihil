#include "AssetPipeline.hpp"

#include "AssetImporter.hpp"
#include "PakWriter.hpp"
#include "Graphics/DefaultResource.hpp"
#include "Graphics/RendererBackend.hpp"

static const std::vector<const char*> sTextureExts { ".png", ".jpg", ".jpeg", ".bmp", ".tga" };
static const std::vector<const char*> sShaderExts { ".glsl", ".hlsl", ".msl" };
static const std::vector<const char*> sMeshExts {".obj", ".fbx", ".gltf", ".glb", ".dae" };

AssetPipeline::AssetPipeline(const std::filesystem::path& root)
    : mRoot{root}
    , mManifest{root / "manifest.txt"}
{
}

void AssetPipeline::BuildAll(const std::filesystem::path& outputFile, u32 compressionLevel, u32 compressionThreshold)
{
    PakWriter pakWriter(outputFile, compressionLevel, compressionThreshold);
    mManifest.Clear();

    // Build Default Materials
    MaterialSpecification defaultMatSpec {
        .UUID = DefaultResource::ObjectMaterial,
        .VertexShaderUUID = DefaultResource::ObjectVertexShader,
        .FragmentShaderUUID = DefaultResource::ObjectFragmentShader,
        .Layout = {
            { "uBaseColor", MaterialParameter::Type::Vec4, 0 },
        }
    };
    mManifest.AddAsset(DefaultResource::ObjectMaterialName.data(), defaultMatSpec.UUID);
    pakWriter.Serialize<MaterialSpecification>(defaultMatSpec);

    MaterialSpecification uiBaseMatSpec {
        .UUID = DefaultResource::UIMaterial,
        .VertexShaderUUID = DefaultResource::UIVertexShader,
        .FragmentShaderUUID = DefaultResource::UIFragmentShader,
        .Layout = {
            { "uBaseColor", MaterialParameter::Type::Vec4, 0 },
        }
    };
    mManifest.AddAsset(DefaultResource::UIMaterialName.data(), uiBaseMatSpec.UUID);
    pakWriter.Serialize<MaterialSpecification>(uiBaseMatSpec);

    // Default meshes
    MeshSpecification uiMeshCreateInfo {
        .UUID = DefaultResource::QuadMesh,
        .Vertices = {
            {.Position = {0.0f, 0.0f, 0.0f}, .TexCoord = {0.0f, 0.0f}},
            {.Position = {1.0f, 0.0f, 0.0f}, .TexCoord = {1.0f, 0.0f}},
            {.Position = {1.0f, 1.0f, 0.0f}, .TexCoord = {1.0f, 1.0f}},
            {.Position = {0.0f, 1.0f, 0.0f}, .TexCoord = {0.0f, 1.0f}},
        },
        .Indices = {
            0, 1, 2,
            0, 2, 3
        },
        .SubMeshes = {
            {.MaterialIndex = 0, .BaseVertex = 0, .BaseIndex = 0, .IndexCount = 6}
        },
        .Materials = {}
    };
    mManifest.AddAsset("Quad", uiMeshCreateInfo.UUID);
    pakWriter.Serialize<MeshSpecification>(uiMeshCreateInfo);

    for (const auto& entry: std::filesystem::recursive_directory_iterator(mRoot / "Textures"))
    {
        if (!entry.is_regular_file())
            continue;

        auto& path = entry.path();
        auto extension = path.extension();
        auto assetName = entry.path().lexically_relative(mRoot / "Textures").generic_string();

        if (mManifest.HasAsset(assetName))
            continue;

        if (std::find(sTextureExts.begin(), sTextureExts.end(), extension) != sTextureExts.end())
        {
            auto textureSpec = AssetImporter::ImportTexture(path);

            if (assetName == DefaultResource::TextureFile)
            {
                textureSpec.UUID = DefaultResource::Texture;
            }

            mManifest.AddAsset(assetName, textureSpec.UUID);
            pakWriter.Serialize<TextureSpecification>(textureSpec);
        }
    }

    for (const auto& entry: std::filesystem::recursive_directory_iterator(mRoot / "Shaders"))
    {
        if (!entry.is_regular_file())
            continue;

        auto& path = entry.path();
        auto extension = path.extension();
        auto assetName = entry.path().stem().string();

        if (mManifest.HasAsset(assetName))
            continue;

        if (std::find(sShaderExts.begin(), sShaderExts.end(), extension) != sShaderExts.end())
        {
            auto shaderSpec = AssetImporter::ImportShader(path);

            // TODO: this is voodoo, but the question is, is it too much voodoo for our purposes? - TD
            if (assetName == DefaultResource::ObjectVertexShaderFile)
            {
                shaderSpec.UUID = DefaultResource::ObjectVertexShader;
            }
            else if (assetName == DefaultResource::ObjectFragmentShaderFile)
            {
                shaderSpec.UUID = DefaultResource::ObjectFragmentShader;
            }
            else if (assetName == DefaultResource::UIVertexShaderFile)
            {
                shaderSpec.UUID = DefaultResource::UIVertexShader;
            }
            else if (assetName == DefaultResource::UIFragmentShaderFile)
            {
                shaderSpec.UUID = DefaultResource::UIFragmentShader;
            }

            mManifest.AddAsset(assetName, shaderSpec.UUID);
            pakWriter.Serialize<ShaderSpecification>(shaderSpec);
        }
    }

    // Import models last to ensure all textures and shaders are already in the manifest
    for (const auto& entry: std::filesystem::recursive_directory_iterator(mRoot / "Models"))
    {
        if (!entry.is_regular_file())
            continue;

        auto& path = entry.path();
        auto extension = path.extension();
        auto assetName = entry.path().stem().string();

        if (mManifest.HasAsset(assetName))
            continue;

        if (std::find(sMeshExts.begin(), sMeshExts.end(), extension) != sMeshExts.end())
        {
            auto importedMesh = AssetImporter::ImportMesh(path);

            MeshSpecification meshSpec {};
            meshSpec.UUID = Nihil::UUID::Generate();
            meshSpec.Vertices = std::move(importedMesh.Vertices);
            meshSpec.Indices = std::move(importedMesh.Indices);
            meshSpec.SubMeshes = std::move(importedMesh.SubMeshes);
            meshSpec.Materials.resize(importedMesh.Materials.size());
            for (size_t i = 0; i < importedMesh.Materials.size(); ++i)
            {
                auto& importedMat = importedMesh.Materials[i];
                MaterialInstanceSpecification materialSpec {};
                materialSpec.UUID = Nihil::UUID::Generate();
                materialSpec.BaseMaterialUUID = DefaultResource::ObjectMaterial;
                meshSpec.Materials[i] = materialSpec.UUID;

                // TODO: Determine slot by reflection on the shaders
                for (i32 j = 0; j < static_cast<i32>(importedMat.TextureNames.size()); ++j)
                {
                    materialSpec.Textures[j] = mManifest.HasAsset(importedMat.TextureNames[j])
                                                 ? mManifest.GetUUID(importedMat.TextureNames[j])
                                                 : DefaultResource::Texture;
                }

                mManifest.AddAsset(std::format("{}/mat_{}", assetName, importedMat.Name), materialSpec.UUID);
                pakWriter.Serialize<MaterialInstanceSpecification>(materialSpec);
            }

            mManifest.AddAsset(assetName, meshSpec.UUID);
            pakWriter.Serialize<MeshSpecification>(meshSpec);
        }
    }

    mManifest.Save();
}

bool AssetPipeline::ValidateManifest()
{
    for (const auto& entry: std::filesystem::recursive_directory_iterator(mRoot / "Textures"))
    {
        if (!entry.is_regular_file())
            continue;

        auto& path = entry.path();
        auto extension = path.extension();
        auto assetName = entry.path().lexically_relative(mRoot / "Textures").generic_string();

        if (std::find(sTextureExts.begin(), sTextureExts.end(), extension) != sTextureExts.end())
        {
            if (!mManifest.HasAsset(assetName))
                return false;
        }
    }

    for (const auto& entry: std::filesystem::recursive_directory_iterator(mRoot / "Shaders"))
    {
        if (!entry.is_regular_file())
            continue;

        auto& path = entry.path();
        auto extension = path.extension();
        auto assetName = entry.path().stem().string();

        if (std::find(sShaderExts.begin(), sShaderExts.end(), extension) != sShaderExts.end())
        {
            if (!mManifest.HasAsset(assetName))
                return false;
        }
    }

    for (const auto& entry: std::filesystem::recursive_directory_iterator(mRoot / "Models"))
    {
        if (!entry.is_regular_file())
            continue;

        auto& path = entry.path();
        auto extension = path.extension();
        auto assetName = entry.path().stem().string();

        if (std::find(sMeshExts.begin(), sMeshExts.end(), extension) != sMeshExts.end())
        {
            if (!mManifest.HasAsset(assetName))
                return false;
        }
    }

    if (!mManifest.HasAsset(DefaultResource::ObjectMaterialName.data()))
        return false;

    return true;
}

auto AssetPipeline::GetManifest() -> const Manifest&
{
    return mManifest;
}