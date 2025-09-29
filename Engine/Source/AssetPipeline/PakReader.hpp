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

    template <typename T> requires IsAnyOf<T, TextureSpecification, ShaderSpecification, MeshSpecification>
    auto Read(std::string_view name) const -> T
    {
        u64 hash = std::hash<std::string_view>{}(name);
        Ensure(mEntryMap.contains(hash), "Asset {} not found in NPak file.", name);
        auto& entry = mEntryMap.at(hash);

        std::vector<std::byte> data;
        data.resize(entry.Size);
        std::memcpy(data.data(), mFile.GetData() + entry.Offset, entry.Size);
        const std::byte* dataPtr = data.data();

        T spec {};
        spec.Name = name;
        if constexpr (std::same_as<T, TextureSpecification>)
        {
            Ensure(entry.Type == PakEntry::Type::Texture, "Asset {} is not a texture.", name);

            TextureHeader header {};
            std::memcpy(&header, data.data(), sizeof(header));
            dataPtr += sizeof(header);
            auto dataBlobSize = entry.Size - sizeof(header);

            spec.Width = header.Width;
            spec.Height = header.Height;
            if (entry.CompressionLevel > 0)
            {
                Logger::Info("Decompressing texture '{}' ({} bytes) with level {}", name, entry.Size, entry.CompressionLevel);
                spec.Data = ZSTD::Decompress({ dataPtr, dataBlobSize });
            }
            else
            {
                spec.Data.resize(dataBlobSize);
                std::memcpy(spec.Data.data(), dataPtr, dataBlobSize);
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

            MeshHeader header {};
            std::memcpy(&header, data.data(), sizeof(header));
            dataPtr += sizeof(header);

            if (entry.CompressionLevel > 0)
            {
                Logger::Info("Decompressing mesh '{}' ({} bytes) with level {}", name, entry.Size, entry.CompressionLevel);
                auto vertexBlob = ZSTD::Decompress({ dataPtr, header.VertexBlobSize });
                dataPtr += header.VertexBlobSize;
                auto indexBlob = ZSTD::Decompress({ dataPtr, header.IndexBlobSize });
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
            }
        }
        return spec;
    }
private:
    std::unordered_map<u64, PakEntry> mEntryMap;
    MappedFile mFile;
};