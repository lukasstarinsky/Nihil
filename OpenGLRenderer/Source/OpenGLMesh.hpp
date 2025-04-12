#pragma once

#include "OpenGLCommon.hpp"
#include "OpenGLMaterial.hpp"
#include "OpenGLBuffer.hpp"

class OpenGLMesh : public Mesh
{
public:
    OpenGLMesh(std::span<const Vertex> vertices, std::span<const Index> indices);
    ~OpenGLMesh() override;

    void Bind() const override;

    auto GetIndexCount() const -> i32 override;
private:
    GLuint mVertexArray {};
    OpenGLBuffer mVertexBuffer;
    OpenGLBuffer mIndexBuffer;
    i32 mIndexCount {};
};