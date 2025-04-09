#include "OpenGLMesh.hpp"

// TODO: temp
static constexpr Vertex vertexData[] = {
    { .Position = { -0.5f, 0.5f, 0.0f }, .Color = { 1.0f, 0.0f, 0.0f } },
    { .Position = { -0.5f, -0.5f, 0.0f }, .Color = { 1.0f, 0.0f, 0.0f } },

    { .Position = { 0.5f, 0.5f, 0.0f }, .Color = { 0.0f, 1.0f, 0.0f } },
    { .Position = { 0.5f, -0.5f, 0.0f}, .Color = { 0.0f, 0.0f, 1.0f } }
};

static constexpr u32 indexData[] = {
    0, 1, 2, 2, 3, 1
};

OpenGLMesh::OpenGLMesh()
    : mVertexBuffer{BufferType::Vertex, vertexData, sizeof(vertexData)}
    , mIndexBuffer{BufferType::Index, indexData, sizeof(indexData)}
    , mIndexCount{sizeof(indexData) / sizeof(u32)}
    , mMaterial{Renderer::DefaultMaterial()}
//    , mMaterial{std::static_pointer_cast<OpenGLMaterial>(Renderer::DefaultMaterial())}
{
    glCreateVertexArrays(1, &mVertexArray);

    glVertexArrayVertexBuffer(mVertexArray, 0, mVertexBuffer.GetHandle(), 0, sizeof(Vertex));
    glVertexArrayElementBuffer(mVertexArray, mIndexBuffer.GetHandle());

    glEnableVertexArrayAttrib(mVertexArray, 0);
    glEnableVertexArrayAttrib(mVertexArray, 1);

    glVertexArrayAttribFormat(mVertexArray, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Position));
    glVertexArrayAttribFormat(mVertexArray, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Color));

    glVertexArrayAttribBinding(mVertexArray, 0, 0);
    glVertexArrayAttribBinding(mVertexArray, 1, 0);
}

OpenGLMesh::~OpenGLMesh()
{
    glDeleteVertexArrays(1, &mVertexArray);
}

auto OpenGLMesh::GetIndexCount() const -> i32
{
    return mIndexCount;
}

void OpenGLMesh::Bind() const
{
    mMaterial->Bind();
    glBindVertexArray(mVertexArray);
}