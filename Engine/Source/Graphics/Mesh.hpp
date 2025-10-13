#pragma once

#include "Vertex.hpp"
#include "Resource.hpp"
#include "Buffer.hpp"
#include "MaterialInstance.hpp"
#include "Platform/UUID.hpp"

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
    VertexLayout VertexLayout {};
    std::vector<SubMesh> SubMeshes;
    std::vector<Nihil::UUID> Materials;
};

struct MeshCreateInfo
{
    std::vector<Vertex> Vertices;
    std::vector<Index> Indices;
    VertexLayout VertexLayout {};
    std::vector<SubMesh> SubMeshes;
    std::vector<MaterialInstancePtr> Materials;
};

class Mesh;
using MeshPtr = std::shared_ptr<Mesh>;

class Mesh : public Resource
{
public:
    using Specification = MeshSpecification;

    virtual ~Mesh() = default;

    virtual void Bind() const = 0;
    virtual auto SetVertexBuffer(const Buffer* vertexBuffer, const VertexLayout& layout, u32 binding) -> void = 0;
    virtual auto GetMaterial(u32 index) const -> const MaterialInstancePtr& = 0;
    virtual auto GetSubMeshes() const -> const std::vector<SubMesh>& = 0;
    virtual auto GetIndexCount() const -> u32 = 0;

    static auto Create(const MeshCreateInfo& createInfo) -> MeshPtr;
};