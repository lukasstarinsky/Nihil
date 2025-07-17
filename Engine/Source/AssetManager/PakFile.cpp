#include "PakFile.hpp"

#pragma region PakReader
PakReader::PakReader(const std::filesystem::path& path)
    : mFile(path)
{
    PakHeader header {};
    std::memcpy(&header, mFile.GetData(), sizeof(PakHeader));
    Ensure(std::memcmp(header.Magic, "NPAK", 4) == 0, "Invalid NPAK magic in file '{}'", path.string());

    std::vector<PakEntry> entries;
    entries.resize(header.AssetCount);
    std::memcpy(entries.data(), mFile.GetData() + header.EntryOffset, sizeof(PakEntry) * header.AssetCount);

    for (const auto& entry: entries)
    {
        mEntryMap[entry.NameHash] = entry;
    }
}

auto PakReader::operator[](std::string_view name) const -> const PakEntry&
{
    u64 hash = std::hash<std::string_view>{}(name);
    Ensure(mEntryMap.contains(hash), "Asset {} not found in NPak file.", name);
    return mEntryMap.at(hash);
}

auto PakReader::operator[](std::string_view name) -> PakEntry&
{
    u64 hash = std::hash<std::string_view>{}(name);
    Ensure(mEntryMap.contains(hash), "Asset {} not found in NPak file.", name);
    return mEntryMap[hash];
}

auto PakReader::GetData(const PakEntry& entry) const -> std::vector<std::byte>
{
    std::vector<std::byte> data;
    data.resize(entry.Size);
    std::memcpy(data.data(), mFile.GetData() + entry.Offset, entry.Size);
    return data;
}
#pragma endregion

#pragma region PakWriter
PakWriter::PakWriter(const std::filesystem::path& path)
    : mHeader{}
    , mFile(path, std::ios::binary)
{
    Ensure(mFile.is_open(), "Failed to open NPak file {} for writing.", path.string());

    // Write dummy header to allow tellp() to work correctly
    mFile.write(reinterpret_cast<const char*>(&mHeader), sizeof(PakHeader));
}

PakWriter::~PakWriter()
{
    if (mFile.is_open())
    {
        Save();
    }
}

void PakWriter::Save()
{
    mHeader.AssetCount = mEntries.size();
    mHeader.EntryOffset = mFile.tellp();

    for (const auto& entry: mEntries)
    {
        mFile.write(reinterpret_cast<const char*>(&entry), sizeof(PakEntry));
    }

    mFile.seekp(0);
    mFile.write(reinterpret_cast<const char*>(&mHeader), sizeof(PakHeader));
    mFile.close();
}
#pragma endregion