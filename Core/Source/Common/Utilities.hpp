#pragma once

#include <string>
#include <fstream>
#include <filesystem>

#include "Defines.hpp"
#include "Exception.hpp"

class NIHIL_API Time
{
public:
    static auto GetFormattedTime(bool includeDate = false) -> std::string;
};

class NIHIL_API File
{
public:
    static auto Read(const std::filesystem::path& filePath) -> std::string;
    static auto ReadLines(const std::filesystem::path& filePath) -> std::vector<std::string>;

    template <typename T = std::byte>
    static auto ReadBinary(const std::filesystem::path& filePath) -> std::vector<T>
    {
        std::ifstream file(filePath, std::ios::binary | std::ios::ate);
        Ensure(file.is_open(), "Failed to open file: {}", filePath.string());

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<T> buffer(size / sizeof(T));
        Ensure(!!file.read(reinterpret_cast<char*>(buffer.data()), size), "Failed to read binary from file: {}", filePath.string());

        return buffer;
    }

    static void WriteBinary(std::string_view filePath, const void* data, std::size_t size);
};