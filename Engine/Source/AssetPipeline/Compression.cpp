#include "Compression.hpp"

#include <zstd.h>

auto ZSTD::Compress(std::span<const std::byte> data, i32 compressionLevel) -> std::vector<std::byte>
{
    auto maxCompressedSize = ZSTD_compressBound(data.size());
    std::vector<std::byte> compressedData(maxCompressedSize);

    auto compressedSize = ZSTD_compress(compressedData.data(), maxCompressedSize, data.data(), data.size(), compressionLevel);
    Ensure(ZSTD_isError(compressedSize) == 0, "ZSTD compression failed: {}", ZSTD_getErrorName(compressedSize));
    compressedData.resize(compressedSize);

    return compressedData;
}

auto ZSTD::Decompress(std::span<const std::byte> data) -> std::vector<std::byte>
{
    auto decompressedSize = ZSTD_getFrameContentSize(data.data(), data.size());
    std::vector<std::byte> decompressedData(decompressedSize);

    auto result = ZSTD_decompress(decompressedData.data(), decompressedData.size(), data.data(), data.size());
    Ensure(ZSTD_isError(result) == 0, "ZSTD decompression failed: {}", ZSTD_getErrorName(result));
    decompressedData.resize(result);

    return decompressedData;
}