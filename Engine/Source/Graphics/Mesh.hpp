#pragma once

struct Vertex
{
    Vec3f Position {};
    Vec2f TexCoord {};
};

using Index = u32;

class Mesh;
using MeshPtr = std::shared_ptr<Mesh>;

class NIHIL_API Mesh
{
public:
    virtual ~Mesh() = default;

    virtual void Bind() const = 0;
    virtual auto GetIndexCount() const -> i32 = 0;

    static auto Create(std::span<const Vertex> vertices, std::span<const Index> indices) -> MeshPtr;
};