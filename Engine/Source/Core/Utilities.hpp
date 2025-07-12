#pragma once

#include <string>
#include <fstream>

#include "Defines.hpp"
#include "Exception.hpp"

namespace Time
{
    auto NIHIL_API GetFormattedTime(bool includeDate = false) -> std::string;
}

namespace File
{
    auto NIHIL_API Read(std::string_view filePath) -> std::string;
    auto NIHIL_API ReadLines(const std::filesystem::path& filePath) -> std::vector<std::string>;

    template <typename T = std::byte>
    auto NIHIL_API ReadBinary(std::string_view filePath) -> std::vector<T>
    {
        std::ifstream file(filePath.data(), std::ios::binary | std::ios::ate);
        Ensure(file.is_open(), "Failed to open file: {}", filePath.data());

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<T> buffer(size / sizeof(T));
        Ensure(!!file.read(reinterpret_cast<char*>(buffer.data()), size), "Failed to read binary from file: {}", filePath.data());

        return buffer;
    }

    void NIHIL_API WriteBinary(std::string_view filePath, const void* data, std::size_t size);
}