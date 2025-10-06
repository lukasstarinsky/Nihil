#include "OpenGLMesh.hpp"

OpenGLMesh::OpenGLMesh(const MeshSpecification& meshSpec)
    : Mesh{meshSpec}
    , mVertexBuffer{BufferType::Vertex, meshSpec.Vertices.data(), static_cast<i32>(meshSpec.Vertices.size() * sizeof(Vertex))}
    , mIndexBuffer{BufferType::Index, meshSpec.Indices.data(), static_cast<i32>(meshSpec.Indices.size() * sizeof(Index))}
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