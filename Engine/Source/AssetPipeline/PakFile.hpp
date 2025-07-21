#pragma once

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

struct TextureHeader
{
    i32 Width;
    i32 Height;
};

struct MeshHeader
{
    u32 VertexBlobSize;
    u32 IndexBlobSize;
};
#pragma pack(pop)