#include "OpenGLBuffer.hpp"

OpenGLBuffer::OpenGLBuffer(BufferType bufferType, const void* data, i32 size, i32 uniformBinding)
    : mBufferType{bufferType}
{
    glCreateBuffers(1, &mHandle);
    if (bufferType == BufferType::Uniform)
    {
        glNamedBufferStorage(mHandle, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
        glBindBufferBase(GL_UNIFORM_BUFFER, uniformBinding, mHandle);
    }
    else
    {
        glNamedBufferStorage(mHandle, size, data, 0);
    }
}

OpenGLBuffer::~OpenGLBuffer()
{
    glDeleteBuffers(1, &mHandle);
}

void OpenGLBuffer::SetData(const void* data, i32 size, i32 offset) const
{
    assert(mBufferType == BufferType::Uniform);
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