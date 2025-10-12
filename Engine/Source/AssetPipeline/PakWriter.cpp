#include "PakWriter.hpp"

PakWriter::PakWriter(const std::filesystem::path& path, u32 compressionLevel, u32 compressionThreshold)
    : mHeader{}
    , mPath{path}
    , mBlobFile{path, std::ios::binary}
    , mMetaFile{path.string() + ".meta", std::ios::binary}
    , mCompressionLevel{compressionLevel}
    , mCompressionThreshold{compressionThreshold}
{
    Ensure(mBlobFile.is_open(), "Failed to open NPAK file {} for writing.", path.string());
    Ensure(mMetaFile.is_open(), "Failed to open NPAK meta file {} for writing.", path.string() + ".meta");

    // Write dummy header to allow tellp() to work correctly
    mMetaFile.write(reinterpret_cast<const char*>(&mHeader), sizeof(PakHeader));
}

PakWriter::~PakWriter()
{
    if (mBlobFile.is_open() && mMetaFile.is_open())
    {
        Save();
    }
}

void PakWriter::Save()
{
    if (mEntries.empty())
    {
        mBlobFile.close();
        mMetaFile.close();
        std::filesystem::remove(mPath);
        std::filesystem::remove(mPath.string() + ".meta");
        return;
    }

    mHeader.AssetCount = mEntries.size();

    for (const auto& entry: mEntries)
    {
        mMetaFile.write(reinterpret_cast<const char*>(&entry), sizeof(PakEntry));
    }

    mMetaFile.seekp(0);
    mMetaFile.write(reinterpret_cast<const char*>(&mHeader), sizeof(PakHeader));
    mMetaFile.close();
    mBlobFile.close();
}

void PakWriter::WriteRaw(PakEntry& entry, const void* data, size_t size)
{
    mBlobFile.write(reinterpret_cast<const char*>(data), static_cast<i32>(size));
    entry.Size += size;
}