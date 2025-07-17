#pragma once

#include <span>

namespace ZSTD
{
    auto Compress(const std::vector<std::byte>& data, i32 compressionLevel) -> std::vector<std::byte>;
    auto Decompress(std::span<std::byte> data) -> std::vector<std::byte>;
}