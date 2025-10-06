#pragma once

#include <filesystem>

class NIHIL_API MappedFile
{
public:
    explicit MappedFile(const std::filesystem::path& filePath);
    ~MappedFile();

    auto GetData() const -> std::byte*;
    auto GetSize() const -> std::size_t;

    void Open();
    void Close() const;
private:
    std::filesystem::path mFilePath {};
    std::byte* mData {};
    std::size_t mSize {};

#ifdef NIHIL_PLATFORM_WINDOWS
    void* mFileHandle {};
    void* mMappingHandle {};
#endif
};