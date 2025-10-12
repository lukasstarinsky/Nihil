#pragma once

#include "OpenGLCommon.hpp"

class OpenGLShader : public Shader
{
public:
    OpenGLShader(const ShaderCreateInfo& createInfo);
    ~OpenGLShader() override;

    auto GetHandle() const -> GLuint;
    auto GetStage() const -> ShaderStage;
private:
    GLuint mHandle {};
    ShaderStage mShaderStage {};
};