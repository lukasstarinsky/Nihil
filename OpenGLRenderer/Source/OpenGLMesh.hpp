#pragma once

#include "OpenGLCommon.hpp"
#include "OpenGLMaterial.hpp"
#include "OpenGLBuffer.hpp"

class OpenGLMesh : public Mesh
{
public:
    explicit OpenGLMesh(const MeshCreateInfo& createInfo);
    ~OpenGLMesh() override;

    void Bind() const override;
    auto GetMaterial(u32 index) const -> const MaterialInstancePtr& override;
    auto GetSubMeshes() const -> const std::vector<SubMesh>& override;
private:
    std::vector<SubMesh> mSubMeshes;
    std::vector<MaterialInstancePtr> mMaterials;
    OpenGLBuffer mVertexBuffer;
    OpenGLBuffer mIndexBuffer;
    GLuint mVertexArray {};
};