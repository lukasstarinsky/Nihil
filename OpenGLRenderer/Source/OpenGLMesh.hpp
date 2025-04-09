#pragma once

#include "OpenGLCommon.hpp"
#include "OpenGLMaterial.hpp"
#include "OpenGLBuffer.hpp"

class OpenGLMesh : public Mesh
{
public:
    OpenGLMesh();
    ~OpenGLMesh() override;

    auto GetIndexCount() const -> i32 override;
    void Bind() const override;
private:
    GLuint mVertexArray {};
    OpenGLBuffer mVertexBuffer;
    OpenGLBuffer mIndexBuffer;
    i32 mIndexCount {};
    MaterialPtr mMaterial {};
};