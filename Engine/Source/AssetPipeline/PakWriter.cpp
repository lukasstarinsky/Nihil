#include "PakWriter.hpp"

PakWriter::PakWriter(const std::filesystem::path& path, i32 compressionLevel, u32 compressionThreshold)
    : mHeader{}
    , mFile(path, std::ios::binary)
    , mCompressionLevel{compressionLevel}
    , mCompressionThreshold{compressionThreshold}
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