#include "OpenGLUniformBuffer.hpp"

OpenGLUniformBuffer::OpenGLUniformBuffer(const UniformBufferCreateInfo& createInfo)
    : mUniformBinding{createInfo.UniformBinding}
{
    glCreateBuffers(1, &mHandle);
    glNamedBufferStorage(mHandle, createInfo.Size, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_UNIFORM_BUFFER, static_cast<GLuint>(createInfo.UniformBinding), mHandle);
}

OpenGLUniformBuffer::~OpenGLUniformBuffer()
{
    glDeleteBuffers(1, &mHandle);
}

void OpenGLUniformBuffer::Bind() const
{
    glBindBufferBase(GL_UNIFORM_BUFFER, static_cast<GLuint>(mUniformBinding), mHandle);
}

void OpenGLUniformBuffer::SetData(const void* data, i32 size, u32 offset) const
{
    glNamedBufferSubData(mHandle, offset, size, data);
}