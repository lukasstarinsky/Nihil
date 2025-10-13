#include "OpenGLBuffer.hpp"

static auto BufferFlagsToGLenum(BufferFlags flags) -> GLbitfield
{
    GLbitfield glFlags = 0;
    if (flags == BufferFlags::DynamicStorage)
    {
        glFlags |= GL_DYNAMIC_STORAGE_BIT;
    }
    return glFlags;
}

OpenGLBuffer::OpenGLBuffer(const BufferCreateInfo& createInfo)
    : mSize{createInfo.Size}
{
    glCreateBuffers(1, &mHandle);
    glNamedBufferStorage(mHandle, createInfo.Size, createInfo.Data, BufferFlagsToGLenum(createInfo.Flags));
}

OpenGLBuffer::~OpenGLBuffer()
{
    glDeleteBuffers(1, &mHandle);
}

void OpenGLBuffer::Bind() const
{
}

void OpenGLBuffer::SetData(const void* data, i32 size, i32 offset) const
{
    glNamedBufferSubData(mHandle, offset, size, data);
}

auto OpenGLBuffer::GetSize() const -> i32
{
    return mSize;
}

auto OpenGLBuffer::GetHandle() const -> GLuint
{
    return mHandle;
}