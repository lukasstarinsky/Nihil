#pragma once

#include "OpenGLCommon.hpp"
#include "OpenGLMaterial.hpp"
#include "OpenGLBuffer.hpp"

class OpenGLMesh : public Mesh
{
public:
    OpenGLMesh(const MeshSpecification& meshSpec);
    ~OpenGLMesh() override;

    void Bind() const override;

    auto GetIndexCount() const -> i32 override;
private:
    GLuint mVertexArray {};
    OpenGLBuffer mVertexBuffer;
    OpenGLBuffer mIndexBuffer;
    i32 mIndexCount {};
};