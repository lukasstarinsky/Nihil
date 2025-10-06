#pragma once

#include "Platform/UUID.hpp"

struct Vertex
{
    Vec3f Position {};
    Vec2f TexCoord {};
};

using Index = u32;

struct SubMesh
{
    u32 BaseVertex {};
    u32 BaseIndex {};
    u32 IndexCount {};
};

struct MeshSpecification
{
    Nihil::UUID UUID {};
    std::vector<Vertex> Vertices;
    std::vector<Index> Indices;
    std::vector<SubMesh> SubMeshes;
};

class Mesh;
using MeshPtr = std::shared_ptr<Mesh>;

class Mesh
{
public:
    virtual ~Mesh() = default;

    virtual void Bind() const = 0;
public:
    auto GetSubMeshes() const -> const std::vector<SubMesh>& { return mSubMeshes; }
    auto GetIndexCount() const -> u32 { return mIndexCount; }

    static auto Create(const MeshSpecification& meshSpec) -> MeshPtr;
protected:
    explicit Mesh(const MeshSpecification& meshSpec)
        : mSubMeshes{meshSpec.SubMeshes}
        , mIndexCount{static_cast<u32>(meshSpec.Indices.size())}
    {

    }

    std::vector<SubMesh> mSubMeshes;
    u32 mIndexCount {};
};