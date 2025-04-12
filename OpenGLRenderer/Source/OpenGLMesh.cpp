#include "OpenGLMesh.hpp"

OpenGLMesh::OpenGLMesh(std::span<const Vertex> vertices, std::span<const Index> indices)
    : mVertexBuffer{BufferType::Vertex, vertices.data(), static_cast<i32>(vertices.size_bytes())}
    , mIndexBuffer{BufferType::Index, indices.data(), static_cast<i32>(indices.size_bytes())}
    , mIndexCount{static_cast<i32>(indices.size())}
    , mMaterial{Renderer::DefaultMaterial()}
//    , mMaterial{std::static_pointer_cast<OpenGLMaterial>(Renderer::DefaultMaterial())}
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
    mMaterial->Bind();
    glBindVertexArray(mVertexArray);
}

auto OpenGLMesh::GetIndexCount() const -> i32
{
    return mIndexCount;
}

auto OpenGLMesh::GetMaterial() const -> const Material*
{
    return mMaterial.get();
}