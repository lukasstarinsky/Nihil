#pragma once

#include <unordered_map>
#include <filesystem>
#include <fstream>

#include "Platform/MappedFile.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Mesh.hpp"

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
    u64 Size;
    u32 OriginalSize;
};
#pragma pack(pop)

class PakReader
{
public:
    explicit PakReader(const std::filesystem::path& path);

    auto operator[](std::string_view name) -> PakEntry&;
    auto operator[](std::string_view name) const -> const PakEntry&;

    auto GetData(const PakEntry& entry) const -> std::vector<std::byte>;
private:
    std::unordered_map<u64, PakEntry> mEntryMap;
    MappedFile mFile;
};

class PakWriter
{
public:
    explicit PakWriter(const std::filesystem::path& path);
    ~PakWriter();

    void Save();

    template <typename T> requires IsAnyOf<T, TextureSpecification, ShaderSpecification, MeshSpecification>
    void Write(const T& spec)
    {
        ASSERT(mFile.is_open());

        PakEntry entry {};
        entry.NameHash = std::hash<std::string_view>{}(spec.Name);
        if constexpr (std::is_same_v<T, TextureSpecification>)
        {
            entry.Type = PakEntry::Type::Texture;
            Write(entry, spec.Data);
        }
        else if constexpr (std::is_same_v<T, ShaderSpecification>)
        {
            entry.Type = PakEntry::Type::Shader;
            Write(entry, spec.Data);
        }
        else if constexpr (std::is_same_v<T, MeshSpecification>)
        {
            auto vertexCount = static_cast<u32>(spec.Vertices.size());
            auto indexCount = static_cast<u32>(spec.Indices.size());

            entry.Type = PakEntry::Type::Mesh;
            entry.Offset = mFile.tellp();
            entry.Size = vertexCount * sizeof(Vertex) + indexCount * sizeof(Index);
            entry.OriginalSize = entry.Size;
            mEntries.push_back(entry);

            mFile.write(reinterpret_cast<const char*>(&vertexCount), sizeof(vertexCount));
            mFile.write(reinterpret_cast<const char*>(spec.Vertices.data()), static_cast<i32>(vertexCount * sizeof(Vertex)));

            mFile.write(reinterpret_cast<const char*>(&indexCount), sizeof(indexCount));
            mFile.write(reinterpret_cast<const char*>(spec.Indices.data()), static_cast<i32>(indexCount * sizeof(Index)));
        }
    }
private:
    void Write(PakEntry& entry, const std::vector<std::byte>& data);
private:
    PakHeader mHeader;
    std::vector<PakEntry> mEntries;
    std::ofstream mFile;
};