#pragma once

#include "OpenGLCommon.hpp"

class OpenGLBuffer : public Buffer
{
public:
    OpenGLBuffer(const BufferCreateInfo& createInfo);
    ~OpenGLBuffer() override;

    void Bind() const override;
    void SetData(const void* data, i32 size, i32 offset) const override;

    auto GetSize() const -> i32 override;
    auto GetHandle() const -> GLuint;
    auto GetType() const -> BufferType;
private:
    i32 mSize {};
    GLuint mHandle {};
    BufferType mBufferType;
    UniformBinding mUniformBinding {};
};