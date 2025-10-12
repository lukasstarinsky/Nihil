#pragma once

#include <filesystem>

#include "AssetManager.hpp"

struct ImportedMaterial
{
    std::string Name;
    std::vector<std::string> TextureNames;
};

struct ImportedMesh
{
    std::vector<Vertex> Vertices;
    std::vector<Index> Indices;
    std::vector<SubMesh> SubMeshes;
    std::vector<ImportedMaterial> Materials;
};

namespace AssetImporter
{
    auto ImportTexture(const std::filesystem::path& path) -> TextureSpecification;
    auto ImportShader(const std::filesystem::path& path) -> ShaderSpecification;
    auto ImportMesh(const std::filesystem::path& path) -> ImportedMesh;
}