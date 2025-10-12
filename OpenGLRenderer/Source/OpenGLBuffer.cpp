#include "OpenGLBuffer.hpp"

OpenGLBuffer::OpenGLBuffer(const BufferCreateInfo& createInfo)
    : mBufferType{createInfo.Type}
{
    glCreateBuffers(1, &mHandle);
    if (mBufferType == BufferType::Uniform)
    {
        glNamedBufferStorage(mHandle, createInfo.Size, nullptr, GL_DYNAMIC_STORAGE_BIT);
        glBindBufferBase(GL_UNIFORM_BUFFER, createInfo.UniformBinding, mHandle);
    }
    else
    {
        glNamedBufferStorage(mHandle, createInfo.Size, createInfo.Data, 0);
    }
}

OpenGLBuffer::~OpenGLBuffer()
{
    glDeleteBuffers(1, &mHandle);
}

void OpenGLBuffer::SetData(const void* data, i32 size, i32 offset) const
{
    ASSERT(mBufferType == BufferType::Uniform);
    glNamedBufferSubData(mHandle, offset, size, data);
}

auto OpenGLBuffer::GetHandle() const -> GLuint
{
    return mHandle;
}

auto OpenGLBuffer::GetType() const -> BufferType
{
    return mBufferType;
}