#pragma once

struct Vertex
{
    Vec3f Position {};
    Vec2f TexCoord {};
};

using Index = u32;

struct MeshSpecification
{
    std::string Name;
    std::vector<Vertex> Vertices;
    std::vector<Index> Indices;
};

class Mesh;
using MeshPtr = std::shared_ptr<Mesh>;

class Mesh
{
public:
    virtual ~Mesh() = default;

    virtual void Bind() const = 0;
    virtual auto GetIndexCount() const -> i32 = 0;

    static auto Create(const MeshSpecification& meshSpec) -> MeshPtr;
};