#pragma once

#include <filesystem>
#include <fstream>

#include "Graphics/Texture.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Mesh.hpp"
#include "Compression.hpp"
#include "PakFile.hpp"

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

            TextureHeader header {
                .Width = spec.Width,
                .Height = spec.Height
            };
            mFile.write(reinterpret_cast<const char*>(&header), sizeof(header));

            if (spec.Data.size() > 0)
            {
                Logger::Info("Compressing texture '{}' ({} bytes) with level {}", spec.Name, spec.Data.size(), mCompressionLevel);
                auto compressedData = ZSTD::Compress(spec.Data, mCompressionLevel);

                entry.CompressionLevel = mCompressionLevel;
                entry.Size = compressedData.size() + sizeof(TextureHeader);
                mFile.write(reinterpret_cast<const char*>(compressedData.data()), static_cast<i32>(compressedData.size()));
            }
            else
            {
                entry.Size = spec.Data.size() + sizeof(TextureHeader);
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

            auto vertexCount = static_cast<u32>(spec.Vertices.size());
            auto indexCount = static_cast<u32>(spec.Indices.size());
            auto uncompressedSize = vertexCount * sizeof(Vertex) + indexCount * sizeof(Index);

            MeshHeader header {};
            if (uncompressedSize > 0)
            {
                Logger::Info("Compressing mesh '{}' ({} bytes) with level {}", spec.Name, uncompressedSize, mCompressionLevel);
                auto compressedVertices = ZSTD::Compress({ reinterpret_cast<const std::byte*>(spec.Vertices.data()), vertexCount * sizeof(Vertex) }, mCompressionLevel);
                auto compressedIndices = ZSTD::Compress({ reinterpret_cast<const std::byte*>(spec.Indices.data()), indexCount * sizeof(Index) }, mCompressionLevel);
                auto compressedVerticesSize = static_cast<u32>(compressedVertices.size());
                auto compressedIndicesSize = static_cast<u32>(compressedIndices.size());

                entry.Size = compressedVertices.size() + compressedIndices.size() + sizeof(header);
                entry.CompressionLevel = mCompressionLevel;
                header.VertexBlobSize = compressedVerticesSize;
                header.IndexBlobSize = compressedIndicesSize;
                mFile.write(reinterpret_cast<const char*>(&header), sizeof(header));
                mFile.write(reinterpret_cast<const char*>(compressedVertices.data()), compressedVerticesSize);
                mFile.write(reinterpret_cast<const char*>(compressedIndices.data()), compressedIndicesSize);
            }
            else
            {
                auto vertexBlobSize = static_cast<u32>(vertexCount * sizeof(Vertex));
                auto indexBlobSize = static_cast<u32>(indexCount * sizeof(Index));

                entry.Size = uncompressedSize + sizeof(header);
                header.VertexBlobSize = vertexBlobSize;
                header.IndexBlobSize = indexBlobSize;
                mFile.write(reinterpret_cast<const char*>(&header), sizeof(header));
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
