#include "Manifest.hpp"

Manifest::Manifest(const std::filesystem::path& filePath)
    : mFilePath{filePath}
{
    Load();
}

void Manifest::AddAsset(const std::string& name, const Nihil::UUID& uuid)
{
    mNameToUUID[name] = uuid;
    mUUIDToName[uuid] = name;
}

auto Manifest::GetUUID(const std::string& name) const -> Nihil::UUID
{
    Ensure(mNameToUUID.contains(name), "Asset name '{}' not found in manifest.", name);
    return mNameToUUID.at(name);
}

auto Manifest::GetName(const Nihil::UUID& uuid) const -> std::string
{
    Ensure(mUUIDToName.contains(uuid), "Asset UUID '{}' not found in manifest.", uuid);
    return mUUIDToName.at(uuid);
}

auto Manifest::GetAllAssets() const -> const std::unordered_map<std::string, Nihil::UUID>&
{
    return mNameToUUID;
}

auto Manifest::HasAsset(const std::string& name) const -> bool
{
    return mNameToUUID.contains(name);
}

auto Manifest::IsEmpty() const -> bool
{
    return !std::filesystem::exists(mFilePath) || std::filesystem::is_empty(mFilePath);
}

void Manifest::Save() const
{
    std::ofstream file(mFilePath, std::ios::trunc);
    Ensure(file.is_open(), "Failed to open manifest file for writing: {}", mFilePath.string());

    for (const auto& [name, uuid] : mNameToUUID)
    {
        file << name << ":" << uuid.ToString() << "\n";
    }
}

void Manifest::Load()
{
    Clear();

    if (!std::filesystem::exists(mFilePath))
        return;

    for (const auto& line : File::ReadLines(mFilePath))
    {
        auto delimiterPos = line.find(':');
        if (delimiterPos == std::string::npos)
        {
            Logger::Warn("Invalid manifest line: {}", line);
            continue;
        }
        std::string name = line.substr(0, delimiterPos);
        Nihil::UUID uuid = Nihil::UUID::FromString(line.substr(delimiterPos + 1));
        AddAsset(name, uuid);
    }
}

void Manifest::Clear()
{
    mNameToUUID.clear();
    mUUIDToName.clear();
}