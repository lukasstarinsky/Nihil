#pragma once

#include "OpenGLCommon.hpp"

class OpenGLShader : public Shader
{
public:
    OpenGLShader(const std::string& filePath, ShaderType shaderType);
    ~OpenGLShader() override;

    auto GetHandle() const -> GLuint;
    auto GetType() const -> ShaderType;
private:
    GLuint mHandle {};
    ShaderType mType {};
};