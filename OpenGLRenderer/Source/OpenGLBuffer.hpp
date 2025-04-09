#pragma once

#include "OpenGLCommon.hpp"

class OpenGLBuffer : public Buffer
{
public:
    OpenGLBuffer(BufferType bufferType, const void* data, i32 size, i32 uniformBinding = 0);
    ~OpenGLBuffer() override;

    void SetData(const void* data, i32 size) const override;

    auto GetHandle() const -> GLuint;
    auto GetType() const -> BufferType;
private:
    GLuint mHandle {};
    BufferType mBufferType;
};