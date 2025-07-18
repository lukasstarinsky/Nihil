#include "PakReader.hpp"

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