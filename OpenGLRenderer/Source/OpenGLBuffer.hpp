#pragma once

#include "OpenGLCommon.hpp"

class OpenGLBuffer : public Buffer
{
public:
    OpenGLBuffer(BufferType bufferType, const void* data, i32 size, i32 uniformBinding);
    ~OpenGLBuffer() override;

    void SetData(const void* data, i32 size) const override;
private:
    GLuint mHandle {};
    BufferType mBufferType;
};