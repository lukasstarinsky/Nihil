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
    explicit PakWriter(const std::filesystem::path& path, u32 compressionLevel, u32 compressionThreshold);
    ~PakWriter();

    void Save();

    template <typename T> requires IsAnyOf<T, TextureSpecification, ShaderSpecification, MeshSpecification, MaterialSpecification, MaterialInstanceSpecification>
    void Write(const T& spec)
    {
        ASSERT(mMetaFile.is_open());
        ASSERT(mBlobFile.is_open());

        PakEntry entry {};
        entry.UUID = spec.UUID;
        entry.Offset = mBlobFile.tellp();
        if constexpr (std::is_same_v<T, TextureSpecification>)
        {
            entry.Type = PakEntry::Type::Texture;

            TextureHeader header {
                .Width = spec.Width,
                .Height = spec.Height
            };
            mBlobFile.write(reinterpret_cast<const char*>(&header), sizeof(header));
            entry.Size = sizeof(TextureHeader);

            if (spec.Data.size() > mCompressionThreshold)
            {
                auto compressedData = ZSTD::Compress(spec.Data, mCompressionLevel);

                entry.CompressionLevel = mCompressionLevel;
                entry.Size += compressedData.size();
                mBlobFile.write(reinterpret_cast<const char*>(compressedData.data()), static_cast<i32>(compressedData.size()));
            }
            else
            {
                entry.Size += spec.Data.size();
                mBlobFile.write(reinterpret_cast<const char*>(spec.Data.data()), static_cast<i32>(spec.Data.size()));
            }
        }
        else if constexpr (std::is_same_v<T, ShaderSpecification>)
        {
            entry.Type = PakEntry::Type::Shader;

            ShaderHeader header {
                .Stage = static_cast<u32>(spec.Stage),
                .VariantCount = static_cast<u32>(spec.Variants.size())
            };
            mBlobFile.write(reinterpret_cast<const char*>(&header), sizeof(header));
            entry.Size = sizeof(ShaderHeader);

            for (const auto& variant: spec.Variants)
            {
                ShaderVariantHeader variantHeader {
                    .API = static_cast<u32>(variant.API),
                    .BlobSize = static_cast<u32>(variant.Data.size())
                };
                mBlobFile.write(reinterpret_cast<const char*>(&variantHeader), sizeof(variantHeader));
                mBlobFile.write(reinterpret_cast<const char*>(variant.Data.data()), static_cast<i32>(variant.Data.size()));
                entry.Size += sizeof(ShaderVariantHeader) + static_cast<i32>(variant.Data.size());
            }
        }
        else if constexpr (std::is_same_v<T, MeshSpecification>)
        {
            entry.Type = PakEntry::Type::Mesh;

            auto vertexCount = static_cast<u32>(spec.Vertices.size());
            auto indexCount = static_cast<u32>(spec.Indices.size());
            auto uncompressedSize = vertexCount * sizeof(Vertex) + indexCount * sizeof(Index);

            MeshHeader header {};
            header.SubMeshCount = static_cast<u32>(spec.SubMeshes.size());
            header.MaterialCount = static_cast<u32>(spec.Materials.size());
            entry.Size = sizeof(MeshHeader);
            if (uncompressedSize > mCompressionThreshold)
            {
                auto compressedVertices = ZSTD::Compress({ reinterpret_cast<const std::byte*>(spec.Vertices.data()), vertexCount * sizeof(Vertex) }, mCompressionLevel);
                auto compressedIndices = ZSTD::Compress({ reinterpret_cast<const std::byte*>(spec.Indices.data()), indexCount * sizeof(Index) }, mCompressionLevel);
                auto compressedVerticesSize = static_cast<u32>(compressedVertices.size());
                auto compressedIndicesSize = static_cast<u32>(compressedIndices.size());

                entry.Size += compressedVertices.size() + compressedIndices.size();
                entry.CompressionLevel = mCompressionLevel;
                header.VertexBlobSize = compressedVerticesSize;
                header.IndexBlobSize = compressedIndicesSize;
                mBlobFile.write(reinterpret_cast<const char*>(&header), sizeof(header));
                mBlobFile.write(reinterpret_cast<const char*>(compressedVertices.data()), compressedVerticesSize);
                mBlobFile.write(reinterpret_cast<const char*>(compressedIndices.data()), compressedIndicesSize);
            }
            else
            {
                auto vertexBlobSize = static_cast<u32>(vertexCount * sizeof(Vertex));
                auto indexBlobSize = static_cast<u32>(indexCount * sizeof(Index));

                entry.Size += uncompressedSize + sizeof(header);
                header.VertexBlobSize = vertexBlobSize;
                header.IndexBlobSize = indexBlobSize;
                mBlobFile.write(reinterpret_cast<const char*>(&header), sizeof(header));
                mBlobFile.write(reinterpret_cast<const char*>(spec.Vertices.data()), vertexBlobSize);
                mBlobFile.write(reinterpret_cast<const char*>(spec.Indices.data()), indexBlobSize);
            }
            entry.Size += sizeof(Nihil::UUID) * header.MaterialCount;
            mBlobFile.write(reinterpret_cast<const char*>(spec.Materials.data()), static_cast<u32>(header.MaterialCount * sizeof(Nihil::UUID)));

            entry.Size += sizeof(SubMesh) * header.SubMeshCount;
            mBlobFile.write(reinterpret_cast<const char*>(spec.SubMeshes.data()), static_cast<u32>(header.SubMeshCount * sizeof(SubMesh)));
        }
        else if constexpr (std::is_same_v<T, MaterialSpecification>)
        {
            entry.Type = PakEntry::Type::Material;
            entry.Size = sizeof(Nihil::UUID) * 2;

            mBlobFile.write(reinterpret_cast<const char*>(&spec.VertexShaderUUID), sizeof(spec.VertexShaderUUID));
            mBlobFile.write(reinterpret_cast<const char*>(&spec.FragmentShaderUUID), sizeof(spec.FragmentShaderUUID));
        }
        else if constexpr (std::is_same_v<T, MaterialInstanceSpecification>)
        {
            entry.Type = PakEntry::Type::MaterialInstance;

            MaterialInstanceHeader header {};
            header.BaseMaterialUUID = spec.BaseMaterialUUID;
            header.TextureCount = static_cast<u32>(spec.Textures.size());
            mBlobFile.write(reinterpret_cast<const char*>(&header), sizeof(header));
            entry.Size = sizeof(MaterialInstanceHeader);

            for (const auto& [slot, uuid]: spec.Textures)
            {
                entry.Size += sizeof(slot);
                entry.Size += sizeof(uuid);
                mBlobFile.write(reinterpret_cast<const char*>(&slot), sizeof(slot));
                mBlobFile.write(reinterpret_cast<const char*>(&uuid), sizeof(uuid));
            }
        }
        mEntries.push_back(entry);
    }
private:
    PakHeader mHeader;
    std::filesystem::path mPath;
    std::vector<PakEntry> mEntries;
    std::ofstream mBlobFile;
    std::ofstream mMetaFile;
    u32 mCompressionLevel;
    u32 mCompressionThreshold;
};
