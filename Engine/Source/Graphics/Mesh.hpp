#pragma once

#include "Resource.hpp"
#include "Material.hpp"
#include "Platform/UUID.hpp"

struct Vertex
{
    Vec3f Position {};
    Vec2f TexCoord {};
};

using Index = u32;

struct SubMesh
{
    u32 MaterialIndex {};
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
    std::vector<MaterialSpecification> Materials;
};

struct MeshCreateInfo
{
    std::vector<Vertex> Vertices;
    std::vector<Index> Indices;
    std::vector<SubMesh> SubMeshes;
    std::vector<MaterialPtr> Materials;
};

class Mesh;
using MeshPtr = std::shared_ptr<Mesh>;

class Mesh : public Resource
{
public:
    using Specification = MeshSpecification;

    virtual ~Mesh() = default;

    virtual void Bind() const = 0;
    virtual auto GetMaterial(u32 index) const -> MaterialPtr = 0;
    virtual auto GetSubMeshes() const -> const std::vector<SubMesh>& = 0;

    static auto Create(const MeshCreateInfo& meshCreateInfo) -> MeshPtr;
};