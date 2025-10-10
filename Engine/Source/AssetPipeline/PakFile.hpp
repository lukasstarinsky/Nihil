#pragma once

#include "Platform/UUID.hpp"

#pragma pack(push, 1)
struct PakHeader
{
    char Magic[4] = {'N', 'P', 'A', 'K'};
    u32 AssetCount;
};

struct PakEntry
{
    enum class Type : u32
    {
        Texture = 0,
        Mesh,
        Shader,
    };

    Nihil::UUID UUID {};
    Type Type;
    u64 Offset;
    u32 CompressionLevel;
    u64 Size;
};

struct TextureHeader
{
    i32 Width;
    i32 Height;
};

struct MeshHeader
{
    u32 VertexBlobSize;
    u32 IndexBlobSize;
    u32 MaterialCount;
    u32 SubMeshCount;
};

struct ShaderHeader
{
    u32 Stage;
    u32 VariantCount;
};

struct ShaderVariantHeader
{
    u32 API;
    u32 BlobSize;
};
#pragma pack(pop)