#pragma once

#include <unordered_map>

#include "Platform/UUID.hpp"

class Manifest
{
public:
    explicit Manifest(const std::filesystem::path& filePath);

    void AddAsset(const std::string& name, const Nihil::UUID& uuid);
    auto GetUUID(const std::string& name) const -> Nihil::UUID;
    auto GetName(const Nihil::UUID& uuid) const -> std::string;
    auto HasAsset(const std::string& name) const -> bool;
    void Save() const;
    void Load();
    void Clear();
private:
    std::unordered_map<std::string, Nihil::UUID> mNameToUUID {};
    std::unordered_map<Nihil::UUID, std::string> mUUIDToName {};
    std::filesystem::path mFilePath {};
};