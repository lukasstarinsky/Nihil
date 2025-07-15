#include "Platform/MappedFile.hpp"

#include "PlatformWin32.hpp"

MappedFile::MappedFile(const std::filesystem::path& filePath)
{
    mFileHandle = CreateFile(
        filePath.string().c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );
    Ensure(mFileHandle != INVALID_HANDLE_VALUE, "Win32: Failed to open file: {}", filePath.string().c_str());

    mSize = GetFileSize(mFileHandle, nullptr);
    Ensure(mSize != INVALID_FILE_SIZE, "Win32: GetFileSize() failed with error code: {}", GetLastError());

    mMappingHandle = CreateFileMapping(
        mFileHandle,
        nullptr,
        PAGE_READONLY,
        0,
        0,
        nullptr
    );
    Ensure(mMappingHandle, "Win32: CreateFileMapping() failed with error code: {}", GetLastError());

    mData = MapViewOfFile(
        mMappingHandle,
        FILE_MAP_READ,
        0,
        0,
        0
    );
    Ensure(mData, "Win32: MapViewOfFile() failed with error code: {}", GetLastError());
}

MappedFile::~MappedFile()
{
    UnmapViewOfFile(mData);
    CloseHandle(mMappingHandle);
    CloseHandle(mFileHandle);
}