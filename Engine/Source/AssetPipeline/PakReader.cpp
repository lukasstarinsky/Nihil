#include "PakReader.hpp"

PakReader::PakReader(const std::filesystem::path& path)
    : mBlobFile{path}
    , mMetaFile{path.string() + ".meta"}
{
    PakHeader header {};
    std::memcpy(&header, mMetaFile.GetData(), sizeof(PakHeader));
    Ensure(std::memcmp(header.Magic, "NPAK", 4) == 0, "Invalid NPAK file: {}", path.string());

    std::vector<PakEntry> entries;
    entries.resize(header.AssetCount);
    std::memcpy(entries.data(), mMetaFile.GetData() + sizeof(PakHeader), header.AssetCount * sizeof(PakEntry));

    for (const auto& entry: entries)
    {
        mEntryMap[entry.UUID] = entry;
    }
}

auto PakReader::HasEntry(const Nihil::UUID& uuid) const -> bool
{
    return mEntryMap.contains(uuid);
}

auto PakReader::ReadRaw(const std::byte*& dataPtr, void* out, size_t size) const -> void
{
    std::memcpy(out, dataPtr, size);
    dataPtr += size;
}