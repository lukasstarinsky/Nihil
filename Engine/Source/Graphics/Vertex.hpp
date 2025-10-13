#pragma once

#include "Math/Math.hpp"

struct Vertex
{
    Vec3f Position {};
    Vec2f TexCoord {};
};

struct VertexAttribute
{
    enum class Type : u32
    {
        // = Component Count
        Float2 = 2,
        Float3 = 3,
        Float4 = 4
    };

    u32 Location {};
    Type Type {};
    u32 Offset {};
    u32 Binding {};
    bool PerInstance {};
};

struct VertexLayout
{
    u32 Stride {};
    std::vector<VertexAttribute> Attributes;

    static auto GetDefault() -> VertexLayout
    {
        return VertexLayout {
            .Stride = sizeof(Vertex),
            .Attributes = {
                { 0, VertexAttribute::Type::Float3, offsetof(Vertex, Position), 0, false },
                { 1, VertexAttribute::Type::Float2, offsetof(Vertex, TexCoord), 0, false },
            }
        };
    }
};