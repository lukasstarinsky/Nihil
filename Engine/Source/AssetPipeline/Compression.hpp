#pragma once

#include <span>

namespace ZSTD
{
    auto Compress(std::span<const std::byte> data, i32 compressionLevel) -> std::vector<std::byte>;
    auto Decompress(std::span<const std::byte> data) -> std::vector<std::byte>;
}