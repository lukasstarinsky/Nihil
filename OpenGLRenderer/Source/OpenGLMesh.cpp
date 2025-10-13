#include "OpenGLMesh.hpp"

static auto VertexAttributeTypeToGLenum(enum VertexAttribute::Type type) -> GLenum
{
    switch (type)
    {
        case VertexAttribute::Type::Float2:
        case VertexAttribute::Type::Float3:
        case VertexAttribute::Type::Float4:
            return GL_FLOAT;
    }

    ASSERT(false, "Unknown VertexAttribute::Type");
    return 0;
}

OpenGLMesh::OpenGLMesh(const MeshCreateInfo& createInfo)
    : mSubMeshes{createInfo.SubMeshes}
    , mMaterials{createInfo.Materials}
    , mVertexBuffer{{BufferType::Vertex, createInfo.Vertices.data(), static_cast<i32>(createInfo.Vertices.size() * sizeof(Vertex))}}
    , mIndexBuffer{{BufferType::Index, createInfo.Indices.data(), static_cast<i32>(createInfo.Indices.size() * sizeof(Index))}}
{
    glCreateVertexArrays(1, &mVertexArray);

    OpenGLMesh::SetVertexBuffer(&mVertexBuffer, createInfo.VertexLayout, 0);
    glVertexArrayElementBuffer(mVertexArray, mIndexBuffer.GetHandle());
}

OpenGLMesh::~OpenGLMesh()
{
    glDeleteVertexArrays(1, &mVertexArray);
}

void OpenGLMesh::Bind() const
{
    glBindVertexArray(mVertexArray);
}

auto OpenGLMesh::SetVertexBuffer(const Buffer* vertexBuffer, const VertexLayout& layout, u32 binding) -> void
{
    auto glBuffer = static_cast<const OpenGLBuffer*>(vertexBuffer);
    glVertexArrayVertexBuffer(mVertexArray, binding, glBuffer->GetHandle(), 0, static_cast<GLsizei>(layout.Stride));

    for (const auto& vertexAttribute: layout.Attributes)
    {
        glEnableVertexArrayAttrib(mVertexArray, vertexAttribute.Location);
        glVertexArrayAttribFormat(mVertexArray, vertexAttribute.Location, static_cast<GLint>(vertexAttribute.Type), VertexAttributeTypeToGLenum(vertexAttribute.Type), GL_FALSE, vertexAttribute.Offset);
        glVertexArrayAttribBinding(mVertexArray, vertexAttribute.Location, vertexAttribute.Binding);
        if (vertexAttribute.PerInstance)
        {
            glVertexArrayBindingDivisor(mVertexArray, vertexAttribute.Binding, 1);
        }
    }
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

auto OpenGLMesh::GetIndexCount() const -> u32
{
    return static_cast<u32>(mIndexBuffer.GetSize() / sizeof(Index));
}