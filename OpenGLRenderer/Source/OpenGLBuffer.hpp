#pragma once

#include "OpenGLCommon.hpp"

class OpenGLBuffer : public Buffer
{
public:
    OpenGLBuffer(const BufferCreateInfo& createInfo);
    ~OpenGLBuffer() override;

    void SetData(const void* data, i32 size, i32 offset) const override;

    auto GetHandle() const -> GLuint;
    auto GetType() const -> BufferType;
private:
    GLuint mHandle {};
    BufferType mBufferType;
};