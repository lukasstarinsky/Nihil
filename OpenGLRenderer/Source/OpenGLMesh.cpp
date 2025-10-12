#include "OpenGLMesh.hpp"

OpenGLMesh::OpenGLMesh(const MeshCreateInfo& createInfo)
    : mSubMeshes{createInfo.SubMeshes}
    , mMaterials{createInfo.Materials}
    , mVertexBuffer{{BufferType::Vertex, createInfo.Vertices.data(), static_cast<i32>(createInfo.Vertices.size() * sizeof(Vertex))}}
    , mIndexBuffer{{BufferType::Index, createInfo.Indices.data(), static_cast<i32>(createInfo.Indices.size() * sizeof(Index))}}
{
    glCreateVertexArrays(1, &mVertexArray);

    glVertexArrayVertexBuffer(mVertexArray, 0, mVertexBuffer.GetHandle(), 0, sizeof(Vertex));
    glVertexArrayElementBuffer(mVertexArray, mIndexBuffer.GetHandle());

    glEnableVertexArrayAttrib(mVertexArray, 0);
    glEnableVertexArrayAttrib(mVertexArray, 1);

    glVertexArrayAttribFormat(mVertexArray, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Position));
    glVertexArrayAttribFormat(mVertexArray, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, TexCoord));

    glVertexArrayAttribBinding(mVertexArray, 0, 0);
    glVertexArrayAttribBinding(mVertexArray, 1, 0);
}

OpenGLMesh::~OpenGLMesh()
{
    glDeleteVertexArrays(1, &mVertexArray);
}

void OpenGLMesh::Bind() const
{
    glBindVertexArray(mVertexArray);
}

auto OpenGLMesh::GetMaterial(u32 index) const -> const MaterialInstancePtr&
{
    ASSERT(index < mMaterials.size() && index >= 0);
    return mMaterials[index];
}

auto OpenGLMesh::GetSubMeshes() const -> const std::vector<SubMesh>&
{
    return mSubMeshes;
}