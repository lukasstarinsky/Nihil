#pragma once

#include <ranges>
#include <unordered_map>
#include <filesystem>
#include <fstream>

#include "Platform/MappedFile.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Mesh.hpp"
#include "Core/Utilities.hpp"
#include "Compression.hpp"

#pragma pack(push, 1)
struct PakHeader
{
    char Magic[4] = {'N', 'P', 'A', 'K'};
    u32 AssetCount;
    u32 EntryOffset;
};

struct PakEntry
{
    enum class Type : u32
    {
        Texture = 0,
        Mesh,
        Shader,
    };

    u64 NameHash;
    Type Type;
    u64 Offset;
    i32 CompressionLevel;
    u64 Size;
};
#pragma pack(pop)

class PakReader
{
public:
    explicit PakReader(const std::filesystem::path& path);

    template <typename T> requires IsAnyOf<T, TextureSpecification, ShaderSpecification, MeshSpecification>
    auto Read(std::string_view name) const -> T
    {
        u64 hash = std::hash<std::string_view>{}(name);
        Ensure(mEntryMap.contains(hash), "Asset {} not found in NPak file.", name);
        auto& entry = mEntryMap.at(hash);

        std::vector<std::byte> data;
        data.resize(entry.Size);
        std::memcpy(data.data(), mFile.GetData() + entry.Offset, entry.Size);

        T spec {};
        spec.Name = name;
        if constexpr (std::same_as<T, TextureSpecification>)
        {
            Ensure(entry.Type == PakEntry::Type::Texture, "Asset {} is not a texture.", name);

            auto metadataSize = sizeof(spec.Width) + sizeof(spec.Height);
            auto dataBlobStart = data.data() + metadataSize;
            auto dataBlobSize = data.size() - metadataSize;
            std::memcpy(&spec.Width, data.data(), sizeof(spec.Width));
            std::memcpy(&spec.Height, data.data() + sizeof(spec.Width), sizeof(spec.Height));

            if (entry.CompressionLevel > 0)
            {
                Logger::Info("Decompressing texture '{}' ({} bytes) with level {}", name, entry.Size, entry.CompressionLevel);
                spec.Data = ZSTD::Decompress({ dataBlobStart, dataBlobSize });
            }
            else
            {
                spec.Data.resize(dataBlobSize);
                std::memcpy(spec.Data.data(), dataBlobStart, dataBlobSize);
            }
        }
        else if constexpr (std::same_as<T, ShaderSpecification>)
        {
            Ensure(entry.Type == PakEntry::Type::Shader, "Asset {} is not a shader.", name);

            spec.Data.resize(entry.Size);
            std::memcpy(spec.Data.data(), data.data(), entry.Size);
        }
        else if constexpr (std::same_as<T, MeshSpecification>)
        {
            Ensure(entry.Type == PakEntry::Type::Mesh, "Asset {} is not a mesh.", name);

            auto metadataSize = sizeof(u32) + sizeof(u32);
            u32 vertexBlobSize;
            u32 indexBlobSize;
            std::memcpy(&vertexBlobSize, data.data(), sizeof(vertexBlobSize));
            std::memcpy(&indexBlobSize, data.data() + sizeof(vertexBlobSize), sizeof(indexBlobSize));
            if (entry.CompressionLevel > 0)
            {
                Logger::Info("Decompressing mesh '{}' ({} bytes) with level {}", name, entry.Size, entry.CompressionLevel);
                auto vertexBlob = ZSTD::Decompress({ data.data() + metadataSize, vertexBlobSize });
                auto indexBlob = ZSTD::Decompress({ data.data() + metadataSize + vertexBlobSize, indexBlobSize });
                auto vertexCount = vertexBlob.size() / sizeof(Vertex);
                auto indexCount = indexBlob.size() / sizeof(Index);

                spec.Vertices.resize(vertexCount);
                spec.Indices.resize(indexCount);
                std::memcpy(spec.Vertices.data(), vertexBlob.data(), vertexBlob.size());
                std::memcpy(spec.Indices.data(), indexBlob.data(), indexBlob.size());
            }
            else
            {
                auto vertexCount = vertexBlobSize / sizeof(Vertex);
                auto indexCount = indexBlobSize / sizeof(Index);

                spec.Vertices.resize(vertexCount);
                spec.Indices.resize(indexCount);
                std::memcpy(spec.Vertices.data(), data.data() + metadataSize, vertexBlobSize);
                std::memcpy(spec.Indices.data(), data.data() + metadataSize + vertexBlobSize, indexBlobSize);
            }
        }
        return spec;
    }
private:
    std::unordered_map<u64, PakEntry> mEntryMap;
    MappedFile mFile;
};

class PakWriter
{
public:
    explicit PakWriter(const std::filesystem::path& path, i32 compressionLevel, u32 compressionThreshold);
    ~PakWriter();

    void Save();

    template <typename T> requires IsAnyOf<T, TextureSpecification, ShaderSpecification, MeshSpecification>
    void Write(const T& spec)
    {
        ASSERT(mFile.is_open());

        PakEntry entry {};
        entry.NameHash = std::hash<std::string_view>{}(spec.Name);
        entry.Offset = mFile.tellp();
        if constexpr (std::is_same_v<T, TextureSpecification>)
        {
            entry.Type = PakEntry::Type::Texture;

            auto metadataSize = sizeof(spec.Width) + sizeof(spec.Height);
            mFile.write(reinterpret_cast<const char*>(&spec.Width), sizeof(spec.Width));
            mFile.write(reinterpret_cast<const char*>(&spec.Height), sizeof(spec.Height));

            if (spec.Data.size() > mCompressionThreshold)
            {
                Logger::Info("Compressing texture '{}' ({} bytes) with level {}", spec.Name, spec.Data.size(), mCompressionLevel);
                auto compressedData = ZSTD::Compress(spec.Data, mCompressionLevel);

                entry.CompressionLevel = mCompressionLevel;
                entry.Size = compressedData.size() + metadataSize;
                mFile.write(reinterpret_cast<const char*>(compressedData.data()), static_cast<i32>(compressedData.size()));
            }
            else
            {
                entry.Size = spec.Data.size() + metadataSize;
                mFile.write(reinterpret_cast<const char*>(spec.Data.data()), static_cast<i32>(spec.Data.size()));
            }
        }
        else if constexpr (std::is_same_v<T, ShaderSpecification>)
        {
            entry.Type = PakEntry::Type::Shader;
            entry.Size = spec.Data.size();

            mFile.write(reinterpret_cast<const char*>(spec.Data.data()), static_cast<i32>(entry.Size));
        }
        else if constexpr (std::is_same_v<T, MeshSpecification>)
        {
            entry.Type = PakEntry::Type::Mesh;

            auto metadataSize = sizeof(u32) + sizeof(u32);
            auto vertexCount = static_cast<u32>(spec.Vertices.size());
            auto indexCount = static_cast<u32>(spec.Indices.size());
            auto uncompressedSize = vertexCount * sizeof(Vertex) + indexCount * sizeof(Index);

            if (uncompressedSize > mCompressionThreshold)
            {
                Logger::Info("Compressing mesh '{}' ({} bytes) with level {}", spec.Name, uncompressedSize, mCompressionLevel);
                auto compressedVertices = ZSTD::Compress({ reinterpret_cast<const std::byte*>(spec.Vertices.data()), vertexCount * sizeof(Vertex) }, mCompressionLevel);
                auto compressedIndices = ZSTD::Compress({ reinterpret_cast<const std::byte*>(spec.Indices.data()), indexCount * sizeof(Index) }, mCompressionLevel);
                auto compressedVerticesCount = static_cast<u32>(compressedVertices.size());
                auto compressedIndicesCount = static_cast<u32>(compressedIndices.size());

                entry.Size = compressedVertices.size() + compressedIndices.size() + metadataSize;
                entry.CompressionLevel = mCompressionLevel;
                mFile.write(reinterpret_cast<const char*>(&compressedVerticesCount), sizeof(compressedVerticesCount));
                mFile.write(reinterpret_cast<const char*>(&compressedIndicesCount), sizeof(compressedIndicesCount));
                mFile.write(reinterpret_cast<const char*>(compressedVertices.data()), compressedVerticesCount);
                mFile.write(reinterpret_cast<const char*>(compressedIndices.data()), compressedIndicesCount);
            }
            else
            {
                auto vertexBlobSize = static_cast<u32>(vertexCount * sizeof(Vertex));
                auto indexBlobSize = static_cast<u32>(indexCount * sizeof(Index));

                entry.Size = uncompressedSize + metadataSize;
                mFile.write(reinterpret_cast<const char*>(&vertexBlobSize), sizeof(vertexBlobSize));
                mFile.write(reinterpret_cast<const char*>(&indexBlobSize), sizeof(indexBlobSize));
                mFile.write(reinterpret_cast<const char*>(spec.Vertices.data()), vertexBlobSize);
                mFile.write(reinterpret_cast<const char*>(spec.Indices.data()), indexBlobSize);
            }
        }
        mEntries.push_back(entry);
    }
private:
    PakHeader mHeader;
    std::vector<PakEntry> mEntries;
    std::ofstream mFile;
    i32 mCompressionLevel;
    u32 mCompressionThreshold;
};