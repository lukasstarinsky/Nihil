#pragma once

#include <filesystem>

#include "AssetManager.hpp"

namespace AssetImporter
{
    auto ImportTexture(const std::filesystem::path& path) -> TextureSpecification;
    auto ImportShader(const std::filesystem::path& path) -> ShaderSpecification;
    auto ImportMesh(const std::filesystem::path& path) -> MeshSpecification;
}