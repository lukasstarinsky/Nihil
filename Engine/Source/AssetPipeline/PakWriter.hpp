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
    void Serialize(const T& spec)
    {
        ASSERT(mMetaFile.is_open());
        ASSERT(mBlobFile.is_open());

        PakEntry entry {};
        entry.UUID = spec.UUID;
        entry.Offset = mBlobFile.tellp();
        if constexpr (std::is_same_v<T, TextureSpecification>)
        {
            entry.Type = PakEntry::Type::Texture;

            TextureEntry header {
                .Width = spec.Width,
                .Height = spec.Height
            };
            Write(entry, header);

            if (spec.Data.size() > mCompressionThreshold)
            {
                auto compressedData = ZSTD::Compress(spec.Data, mCompressionLevel);

                entry.CompressionLevel = mCompressionLevel;
                WriteRaw(entry, compressedData.data(), compressedData.size());
            }
            else
            {
                WriteRaw(entry, spec.Data.data(), spec.Data.size());
            }
        }
        else if constexpr (std::is_same_v<T, ShaderSpecification>)
        {
            entry.Type = PakEntry::Type::Shader;

            ShaderEntry header {
                .Stage = static_cast<u32>(spec.Stage),
                .VariantCount = static_cast<u32>(spec.Variants.size())
            };
            Write(entry, header);

            for (const auto& variant: spec.Variants)
            {
                ShaderVariantEntry variantHeader {
                    .API = static_cast<u32>(variant.API),
                    .BlobSize = static_cast<u32>(variant.Data.size())
                };
                Write(entry, variantHeader);
                WriteRaw(entry, variant.Data.data(), variant.Data.size());
            }
        }
        else if constexpr (std::is_same_v<T, MeshSpecification>)
        {
            entry.Type = PakEntry::Type::Mesh;

            auto vertexCount = static_cast<u32>(spec.Vertices.size());
            auto indexCount = static_cast<u32>(spec.Indices.size());
            auto uncompressedSize = vertexCount * sizeof(Vertex) + indexCount * sizeof(Index);

            MeshEntry header {
                .VertexBlobSize = 0,
                .IndexBlobSize = 0,
                .MaterialCount = static_cast<u32>(spec.Materials.size()),
                .SubMeshCount = static_cast<u32>(spec.SubMeshes.size())
            };
            if (uncompressedSize > mCompressionThreshold)
            {
                auto compressedVertices = ZSTD::Compress({ reinterpret_cast<const std::byte*>(spec.Vertices.data()), vertexCount * sizeof(Vertex) }, mCompressionLevel);
                auto compressedIndices = ZSTD::Compress({ reinterpret_cast<const std::byte*>(spec.Indices.data()), indexCount * sizeof(Index) }, mCompressionLevel);
                auto compressedVerticesSize = static_cast<u32>(compressedVertices.size());
                auto compressedIndicesSize = static_cast<u32>(compressedIndices.size());

                entry.CompressionLevel = mCompressionLevel;
                header.VertexBlobSize = compressedVerticesSize;
                header.IndexBlobSize = compressedIndicesSize;

                Write(entry, header);
                WriteRaw(entry, compressedVertices.data(), compressedVerticesSize);
                WriteRaw(entry, compressedIndices.data(), compressedIndicesSize);
            }
            else
            {
                auto vertexBlobSize = static_cast<u32>(vertexCount * sizeof(Vertex));
                auto indexBlobSize = static_cast<u32>(indexCount * sizeof(Index));

                header.VertexBlobSize = vertexBlobSize;
                header.IndexBlobSize = indexBlobSize;

                Write(entry, header);
                WriteRaw(entry, spec.Vertices.data(), vertexBlobSize);
                WriteRaw(entry, spec.Indices.data(), indexBlobSize);
                entry.Size -= vertexBlobSize + indexBlobSize;
            }
            WriteRaw(entry, spec.Materials.data(), static_cast<u32>(header.MaterialCount * sizeof(Nihil::UUID)));
            WriteRaw(entry, spec.SubMeshes.data(), static_cast<u32>(header.SubMeshCount * sizeof(SubMesh)));
        }
        else if constexpr (std::is_same_v<T, MaterialSpecification>)
        {
            entry.Type = PakEntry::Type::Material;

            MaterialEntry header {
                .VertexShaderUUID = spec.VertexShaderUUID,
                .FragmentShaderUUID = spec.FragmentShaderUUID,
                .ParameterCount = static_cast<u32>(spec.Layout.size())
            };
            Write(entry, header);

            for (const auto& param: spec.Layout)
            {
                MaterialParameterEntry paramEntry {
                    .NameLength = static_cast<u32>(param.Name.size()),
                    .Type = static_cast<u32>(param.Type)
                };
                Write(entry, paramEntry);
                WriteRaw(entry, param.Name.data(), paramEntry.NameLength);
            }
        }
        else if constexpr (std::is_same_v<T, MaterialInstanceSpecification>)
        {
            entry.Type = PakEntry::Type::MaterialInstance;

            MaterialInstanceEntry header {
                .BaseMaterialUUID = spec.BaseMaterialUUID,
                .UniformDataSize = static_cast<u32>(spec.UniformData.size()),
                .TextureCount = static_cast<u32>(spec.Textures.size())
            };
            Write(entry, header);
            WriteRaw(entry, spec.UniformData.data(), spec.UniformData.size());

            for (const auto& [slot, uuid]: spec.Textures)
            {
                MaterialInstanceTextureEntry texEntry {
                    .Slot = slot,
                    .TextureUUID = uuid
                };
                Write(entry, texEntry);
            }
        }
        mEntries.push_back(entry);
    }
private:
    void WriteRaw(PakEntry& entry, const void* data, size_t size);

    template <typename T>
    void Write(PakEntry& entry, const T& data)
    {
        mBlobFile.write(reinterpret_cast<const char*>(&data), sizeof(T));
        entry.Size += sizeof(T);
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
