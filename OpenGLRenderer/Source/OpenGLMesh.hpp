#pragma once

#include "OpenGLCommon.hpp"
#include "OpenGLMaterial.hpp"
#include "OpenGLBuffer.hpp"

class OpenGLMesh : public Mesh
{
public:
    OpenGLMesh();
    ~OpenGLMesh() override;

    void Bind() const override;
private:
    GLuint mVertexArray {};
    OpenGLBuffer mVertexBuffer;
    MaterialPtr mMaterial {};
};