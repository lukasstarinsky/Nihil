#pragma once

#include <filesystem>
#include <unordered_map>

#include "Platform/MappedFile.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Material.hpp"
#include "Common/Utilities.hpp"
#include "Compression.hpp"
#include "PakFile.hpp"

class PakReader
{
public:
    explicit PakReader(const std::filesystem::path& path);

    auto HasEntry(const Nihil::UUID& uuid) const -> bool;

    template <typename T> requires IsAnyOf<T, TextureSpecification, ShaderSpecification, MeshSpecification, MaterialSpecification, MaterialInstanceSpecification>
    auto Deserialize(const Nihil::UUID& uuid) const -> T
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
            auto header = Read<TextureEntry>(dataPtr);
            auto dataBlobSize = entry.Size - sizeof(header);

            spec.Width = header.Width;
            spec.Height = header.Height;
            if (entry.CompressionLevel > 0)
            {
                spec.Data = ZSTD::Decompress({ dataPtr, dataBlobSize });
                dataPtr += dataBlobSize;
            }
            else
            {
                spec.Data.resize(dataBlobSize);
                ReadRaw(dataPtr, spec.Data.data(), dataBlobSize);
            }
        }
        else if constexpr (std::same_as<T, ShaderSpecification>)
        {
            auto header = Read<ShaderEntry>(dataPtr);

            spec.Stage = static_cast<ShaderStage>(header.Stage);
            spec.Variants.resize(header.VariantCount);

            for (u32 i = 0; i < header.VariantCount; ++i)
            {
                auto variantHeader = Read<ShaderVariantEntry>(dataPtr);

                spec.Variants[i].API = static_cast<RendererAPI>(variantHeader.API);
                spec.Variants[i].Data.resize(variantHeader.BlobSize);
                ReadRaw(dataPtr, spec.Variants[i].Data.data(), variantHeader.BlobSize);
            }
        }
        else if constexpr (std::same_as<T, MeshSpecification>)
        {
            auto header = Read<MeshEntry>(dataPtr);

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
                ReadRaw(dataPtr, spec.Vertices.data(), header.VertexBlobSize);

                spec.Indices.resize(indexCount);
                ReadRaw(dataPtr, spec.Indices.data(), header.IndexBlobSize);
            }
            spec.VertexLayout.Stride = header.VertexLayout.Stride;
            spec.VertexLayout.Attributes.resize(header.VertexLayout.AttributeCount);
            ReadRaw(dataPtr, spec.VertexLayout.Attributes.data(), header.VertexLayout.AttributeCount * sizeof(VertexAttributeEntry));

            spec.Materials.resize(header.MaterialCount);
            ReadRaw(dataPtr, spec.Materials.data(), header.MaterialCount * sizeof(Nihil::UUID));

            spec.SubMeshes.resize(header.SubMeshCount);
            ReadRaw(dataPtr, spec.SubMeshes.data(), header.SubMeshCount * sizeof(SubMesh));
        }
        else if constexpr (std::same_as<T, MaterialSpecification>)
        {
            auto header = Read<MaterialEntry>(dataPtr);

            spec.VertexShaderUUID = header.VertexShaderUUID;
            spec.FragmentShaderUUID = header.FragmentShaderUUID;

            for (u32 i = 0; i < header.ParameterCount; ++i)
            {
                auto paramEntry = Read<MaterialParameterEntry>(dataPtr);

                std::string name(paramEntry.NameLength, '\0');
                ReadRaw(dataPtr, name.data(), paramEntry.NameLength);

                MaterialParameter param {
                    .Name = std::move(name),
                    .Type = static_cast<enum class MaterialParameter::Type>(paramEntry.Type),
                    .Offset = paramEntry.Offset
                };
                spec.Layout.push_back(param);
            }
        }
        else if constexpr (std::same_as<T, MaterialInstanceSpecification>)
        {
            auto header = Read<MaterialInstanceEntry>(dataPtr);

            spec.BaseMaterialUUID = header.BaseMaterialUUID;

            spec.UniformData.resize(header.UniformDataSize);
            ReadRaw(dataPtr, spec.UniformData.data(), header.UniformDataSize);

            for (u32 i = 0; i < header.TextureCount; ++i)
            {
                auto texEntry = Read<MaterialInstanceTextureEntry>(dataPtr);
                spec.Textures[texEntry.Slot] = texEntry.TextureUUID;
            }
        }
        return spec;
    }
private:
    void ReadRaw(const std::byte*& dataPtr, void* out, size_t size) const;

    template<typename T>
    auto Read(const std::byte*& dataPtr) const -> T
    {
        T result {};
        std::memcpy(&result, dataPtr, sizeof(T));
        dataPtr += sizeof(T);
        return result;
    }
private:
    std::unordered_map<Nihil::UUID, PakEntry> mEntryMap;
    MappedFile mBlobFile;
    MappedFile mMetaFile;
};