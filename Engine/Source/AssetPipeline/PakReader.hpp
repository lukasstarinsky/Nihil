#pragma once

#include <filesystem>
#include <unordered_map>

#include "Platform/MappedFile.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Mesh.hpp"
#include "Common/Utilities.hpp"
#include "Compression.hpp"
#include "PakFile.hpp"

class PakReader
{
public:
    explicit PakReader(const std::filesystem::path& path);

    auto HasEntry(const Nihil::UUID& uuid) const -> bool;

    template <typename T> requires IsAnyOf<T, TextureSpecification, ShaderSpecification, MeshSpecification, MaterialSpecification, MaterialInstanceSpecification>
    auto Read(const Nihil::UUID& uuid) const -> T
    {
        Ensure(mEntryMap.contains(uuid), "Asset with uuid: '{}' not found in NPAK file.", uuid);
        auto& entry = mEntryMap.at(uuid);

        std::vector<std::byte> data;
        data.resize(entry.Size);
        std::memcpy(data.data(), mBlobFile.GetData() + entry.Offset, entry.Size);
        const std::byte* dataPtr = data.data();

        T spec {};
        spec.UUID = uuid;
        if constexpr (std::same_as<T, TextureSpecification>)
        {
            TextureHeader header {};
            std::memcpy(&header, data.data(), sizeof(header));
            dataPtr += sizeof(header);
            auto dataBlobSize = entry.Size - sizeof(header);

            spec.Width = header.Width;
            spec.Height = header.Height;
            if (entry.CompressionLevel > 0)
            {
                spec.Data = ZSTD::Decompress({ dataPtr, dataBlobSize });
            }
            else
            {
                spec.Data.resize(dataBlobSize);
                std::memcpy(spec.Data.data(), dataPtr, dataBlobSize);
                dataPtr += dataBlobSize;
            }
        }
        else if constexpr (std::same_as<T, ShaderSpecification>)
        {
            ShaderHeader header {};
            std::memcpy(&header, data.data(), sizeof(header));
            dataPtr += sizeof(header);

            spec.Stage = static_cast<ShaderStage>(header.Stage);
            spec.Variants.resize(header.VariantCount);

            for (u32 i = 0; i < header.VariantCount; ++i)
            {
                ShaderVariantHeader variantHeader {};
                std::memcpy(&variantHeader, dataPtr, sizeof(variantHeader));
                dataPtr += sizeof(variantHeader);

                spec.Variants[i].API = static_cast<RendererAPI>(variantHeader.API);
                spec.Variants[i].Data.resize(variantHeader.BlobSize);
                std::memcpy(spec.Variants[i].Data.data(), dataPtr, variantHeader.BlobSize);
                dataPtr += variantHeader.BlobSize;
            }
        }
        else if constexpr (std::same_as<T, MeshSpecification>)
        {
            MeshHeader header {};
            std::memcpy(&header, data.data(), sizeof(header));
            dataPtr += sizeof(header);

            if (entry.CompressionLevel > 0)
            {
                auto vertexBlob = ZSTD::Decompress({ dataPtr, header.VertexBlobSize });
                dataPtr += header.VertexBlobSize;
                auto indexBlob = ZSTD::Decompress({ dataPtr, header.IndexBlobSize });
                dataPtr += header.IndexBlobSize;
                auto vertexCount = vertexBlob.size() / sizeof(Vertex);
                auto indexCount = indexBlob.size() / sizeof(Index);

                spec.Vertices.resize(vertexCount);
                spec.Indices.resize(indexCount);
                std::memcpy(spec.Vertices.data(), vertexBlob.data(), vertexBlob.size());
                std::memcpy(spec.Indices.data(), indexBlob.data(), indexBlob.size());
            }
            else
            {
                auto vertexCount = header.VertexBlobSize / sizeof(Vertex);
                auto indexCount = header.IndexBlobSize / sizeof(Index);

                spec.Vertices.resize(vertexCount);
                spec.Indices.resize(indexCount);
                std::memcpy(spec.Vertices.data(), dataPtr, header.VertexBlobSize);
                dataPtr += header.VertexBlobSize;
                std::memcpy(spec.Indices.data(), dataPtr, header.IndexBlobSize);
                dataPtr += header.IndexBlobSize;
            }
            spec.Materials.resize(header.MaterialCount);
            std::memcpy(spec.Materials.data(), dataPtr, header.MaterialCount * sizeof(Nihil::UUID));
            dataPtr += header.MaterialCount * sizeof(Nihil::UUID);

            spec.SubMeshes.resize(header.SubMeshCount);
            std::memcpy(spec.SubMeshes.data(), dataPtr, header.SubMeshCount * sizeof(SubMesh));
            dataPtr += header.SubMeshCount * sizeof(SubMesh);
        }
        else if constexpr (std::same_as<T, MaterialSpecification>)
        {
            std::memcpy(&spec.VertexShaderUUID, dataPtr, sizeof(Nihil::UUID));
            dataPtr += sizeof(Nihil::UUID);
            std::memcpy(&spec.FragmentShaderUUID, dataPtr, sizeof(Nihil::UUID));
            dataPtr += sizeof(Nihil::UUID);
        }
        else if constexpr (std::same_as<T, MaterialInstanceSpecification>)
        {
            MaterialInstanceHeader header {};
            std::memcpy(&header, data.data(), sizeof(header));
            dataPtr += sizeof(header);

            spec.BaseMaterialUUID = header.BaseMaterialUUID;
            for (u32 i = 0; i < header.TextureCount; ++i)
            {
                u32 slot;
                Nihil::UUID textureUUID {};
                std::memcpy(&slot, dataPtr, sizeof(slot));
                dataPtr += sizeof(slot);
                std::memcpy(&textureUUID, dataPtr, sizeof(textureUUID));
                dataPtr += sizeof(textureUUID);
                spec.Textures[slot] = textureUUID;
            }
        }
        return spec;
    }
private:
    std::unordered_map<Nihil::UUID, PakEntry> mEntryMap;
    MappedFile mBlobFile;
    MappedFile mMetaFile;
};