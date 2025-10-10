#include "AssetPipeline.hpp"

#include "AssetImporter.hpp"
#include "PakWriter.hpp"
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

            if (assetName == "default.png")
            {
                textureSpec.UUID = Nihil::UUID(2);
            }

            mManifest.AddAsset(assetName, textureSpec.UUID);
            pakWriter.Write<TextureSpecification>(textureSpec);
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
            // Maybe make some enum of defaults
            if (assetName == "DefaultObjectShader.vs")
            {
                shaderSpec.UUID = Nihil::UUID(0);
            }
            else if (assetName == "DefaultObjectShader.fs")
            {
                shaderSpec.UUID = Nihil::UUID(1);
            }

            mManifest.AddAsset(assetName, shaderSpec.UUID);
            pakWriter.Write<ShaderSpecification>(shaderSpec);
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
            auto meshSpec = AssetImporter::ImportMesh(path, mManifest);

            for (auto& material : meshSpec.Materials)
            {
                material.VertexShaderUUID = Nihil::UUID(0);
                material.FragmentShaderUUID = Nihil::UUID(1);
            }

            mManifest.AddAsset(assetName, meshSpec.UUID);
            pakWriter.Write<MeshSpecification>(meshSpec);
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
        auto assetName = entry.path().filename().replace_extension().string();

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
        auto assetName = entry.path().filename().replace_extension().string();

        if (std::find(sMeshExts.begin(), sMeshExts.end(), extension) != sMeshExts.end())
        {
            if (!mManifest.HasAsset(assetName))
                return false;
        }
    }

    return true;
}

auto AssetPipeline::GetManifest() -> const Manifest&
{
    return mManifest;
}