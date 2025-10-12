#pragma once

#include "Platform/UUID.hpp"
#include "Graphics/Material.hpp"

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
        Material,
        MaterialInstance
    };

    Nihil::UUID UUID {};
    Type Type;
    u64 Offset;
    u32 CompressionLevel;
    u64 Size {};
};

struct TextureEntry
{
    i32 Width;
    i32 Height;
    // Followed by array of bytes
};

struct MeshEntry
{
    u32 VertexBlobSize;
    u32 IndexBlobSize;
    u32 MaterialCount;
    u32 SubMeshCount;
    // Followed by vertex blob
    // Followed by index blob
    // Followed by array of Material UUIDs
    // Followed by array of SubMeshes
};

struct MaterialParameterEntry
{
    u32 NameLength;
    u32 Type;
    // Followed by array of bytes
};

struct MaterialEntry
{
    Nihil::UUID VertexShaderUUID {};
    Nihil::UUID FragmentShaderUUID {};
    u32 ParameterCount;
    // Followed by array of MaterialParameterEntry
};

struct MaterialInstanceTextureEntry
{
    i32 Slot;
    Nihil::UUID TextureUUID {};
};

struct MaterialInstanceEntry
{
    Nihil::UUID BaseMaterialUUID {};
    u32 UniformDataSize;
    // Followed by array of bytes
    u32 TextureCount;
    // Followed by array of MaterialInstanceTextureEntry
};

struct ShaderEntry
{
    u32 Stage;
    u32 VariantCount;
    // Followed by array of ShaderVariantHeader
};

struct ShaderVariantEntry
{
    u32 API;
    u32 BlobSize;
    // Followed by array of bytes
};
#pragma pack(pop)