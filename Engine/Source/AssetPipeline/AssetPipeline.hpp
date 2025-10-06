#pragma once

#include <filesystem>

#include "AssetManager.hpp"
#include "Manifest.hpp"

class AssetPipeline
{
public:
    explicit AssetPipeline(const std::filesystem::path& root);

    void BuildAll(const std::filesystem::path& outputFile, u32 compressionLevel, u32 compressionThreshold);
    bool ValidateManifest();

    auto GetManifest() -> const Manifest&;
private:
    std::filesystem::path mRoot {};
    Manifest mManifest;
};