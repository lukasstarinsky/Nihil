#include "OpenGLMesh.hpp"

static constexpr Vertex vertexData[] = {
    { .Position = { 0.0f, 0.5f, 0.0f }, .Color = { 1.0f, 0.0f, 0.0f } },
    { .Position = { -0.5f, -0.5f, 0.0f }, .Color = { 0.0f, 1.0f, 0.0f } },
    { .Position = { 0.5f, -0.5f, 0.0f}, .Color = { 0.0f, 0.0f, 1.0f } }
};

OpenGLMesh::OpenGLMesh()
    : mVertexBuffer{BufferType::Vertex, vertexData, sizeof(vertexData)}
    , mMaterial{Renderer::DefaultMaterial()}
//    , mMaterial{std::static_pointer_cast<OpenGLMaterial>(Renderer::DefaultMaterial())}
{
    glCreateVertexArrays(1, &mVertexArray);

    glVertexArrayVertexBuffer(mVertexArray, 0, mVertexBuffer.GetHandle(), 0, sizeof(Vertex));

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

void OpenGLMesh::Bind() const
{
    mMaterial->Bind();
    glBindVertexArray(mVertexArray);
}