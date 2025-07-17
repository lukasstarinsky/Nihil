#pragma once

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

            u32 vertexCount = 0;
            u32 indexCount = 0;
            std::memcpy(&vertexCount, data.data(), sizeof(vertexCount));
            std::memcpy(&indexCount, data.data() + sizeof(vertexCount), sizeof(indexCount));

            spec.Vertices.resize(vertexCount);
            spec.Indices.resize(indexCount);
            std::memcpy(spec.Vertices.data(), data.data() + sizeof(vertexCount) + sizeof(indexCount), vertexCount * sizeof(Vertex));
            std::memcpy(spec.Indices.data(), data.data() + sizeof(vertexCount) + sizeof(indexCount) + vertexCount * sizeof(Vertex), indexCount * sizeof(Index));
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
            auto metadataSize = sizeof(spec.Width) + sizeof(spec.Height);
            entry.Type = PakEntry::Type::Texture;

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
            auto vertexCount = static_cast<u32>(spec.Vertices.size());
            auto indexCount = static_cast<u32>(spec.Indices.size());

            entry.Type = PakEntry::Type::Mesh;
            entry.Size = vertexCount * sizeof(Vertex) + indexCount * sizeof(Index);

            mFile.write(reinterpret_cast<const char*>(&vertexCount), sizeof(vertexCount));
            mFile.write(reinterpret_cast<const char*>(&indexCount), sizeof(indexCount));
            mFile.write(reinterpret_cast<const char*>(spec.Vertices.data()), static_cast<i32>(vertexCount * sizeof(Vertex)));
            mFile.write(reinterpret_cast<const char*>(spec.Indices.data()), static_cast<i32>(indexCount * sizeof(Index)));
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