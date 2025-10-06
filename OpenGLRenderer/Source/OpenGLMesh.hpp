#pragma once

#include "OpenGLCommon.hpp"
#include "OpenGLMaterial.hpp"
#include "OpenGLBuffer.hpp"

class OpenGLMesh : public Mesh
{
public:
    explicit OpenGLMesh(const MeshSpecification& meshSpec);
    ~OpenGLMesh() override;

    void Bind() const override;
private:
    OpenGLBuffer mVertexBuffer;
    OpenGLBuffer mIndexBuffer;
    GLuint mVertexArray {};
};