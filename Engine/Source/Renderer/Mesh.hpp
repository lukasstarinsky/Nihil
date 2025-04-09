#pragma once

#include "Math/Math.hpp"

struct Vertex
{
    Vec3f Position {};
    Vec3f Color {};
};

class Mesh;
using MeshPtr = std::shared_ptr<Mesh>;

class NIHIL_API Mesh
{
public:
    virtual ~Mesh() = default;

    virtual auto GetIndexCount() const -> i32 = 0;
    virtual void Bind() const = 0;

    static auto Create() -> MeshPtr;
};