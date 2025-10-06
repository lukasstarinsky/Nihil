#include "AssetPipeline.hpp"

#include "ShaderCompiler.hpp"
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

    for (const auto& entry: std::filesystem::recursive_directory_iterator(mRoot))
    {
        if (!entry.is_regular_file())
            continue;

        auto& path = entry.path();
        auto extension = path.extension();
        auto assetName = entry.path().filename().replace_extension().string();

        if (mManifest.HasAsset(assetName))
            continue;

        // Scan Textures
        if (std::find(sTextureExts.begin(), sTextureExts.end(), extension) != sTextureExts.end())
        {
            auto textureSpec = AssetImporter::ImportTexture(path);
            mManifest.AddAsset(assetName, textureSpec.UUID);
            pakWriter.Write<TextureSpecification>(textureSpec);
        }
        // Scan Shaders
        else if (std::find(sShaderExts.begin(), sShaderExts.end(), extension) != sShaderExts.end())
        {
            auto shaderSpec = AssetImporter::ImportShader(path);
            mManifest.AddAsset(assetName, shaderSpec.UUID);
            pakWriter.Write<ShaderSpecification>(shaderSpec);
        }
        // Scan Meshes
        else if (std::find(sMeshExts.begin(), sMeshExts.end(), extension) != sMeshExts.end())
        {
            auto meshSpec = AssetImporter::ImportMesh(path);
            mManifest.AddAsset(assetName, meshSpec.UUID);
            pakWriter.Write<MeshSpecification>(meshSpec);
        }
    }

    mManifest.Save();
}

bool AssetPipeline::ValidateManifest()
{
    for (const auto& entry: std::filesystem::recursive_directory_iterator(mRoot))
    {
        if (!entry.is_regular_file())
            continue;

        auto& path = entry.path();
        auto extension = path.extension();
        auto assetName = entry.path().filename().replace_extension().string();

        if (std::find(sTextureExts.begin(), sTextureExts.end(), extension) != sTextureExts.end() ||
            std::find(sShaderExts.begin(), sShaderExts.end(), extension) != sShaderExts.end() ||
            std::find(sMeshExts.begin(), sMeshExts.end(), extension) != sMeshExts.end())
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