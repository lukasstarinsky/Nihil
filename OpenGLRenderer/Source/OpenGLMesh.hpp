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
    std::shared_ptr<OpenGLBuffer> mVertexBuffer {};
    std::shared_ptr<OpenGLMaterial> mMaterial {};
};