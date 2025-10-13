#pragma once

#include "OpenGLCommon.hpp"

class OpenGLUniformBuffer : public UniformBuffer
{
public:
    OpenGLUniformBuffer(const UniformBufferCreateInfo& createInfo);
    ~OpenGLUniformBuffer() override;

    void Bind() const override;
    void SetData(const void* data, i32 size, u32 offset) const override;
private:
    GLuint mHandle {};
    UniformBinding mUniformBinding {};
};