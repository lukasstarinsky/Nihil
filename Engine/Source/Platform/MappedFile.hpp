#pragma once

#include <filesystem>

class NIHIL_API MappedFile
{
public:
    explicit MappedFile(const std::filesystem::path& filePath);
    ~MappedFile();
private:
    void* mData {};
    std::size_t mSize {};

#ifdef NIHIL_PLATFORM_WINDOWS
    void* mFileHandle {};
    void* mMappingHandle {};
#endif
};