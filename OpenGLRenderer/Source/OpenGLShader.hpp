#pragma once

#include "OpenGLCommon.hpp"

class OpenGLShader : public Shader
{
public:
    OpenGLShader(const std::string& filePath, ShaderType shaderType);
    ~OpenGLShader() override;
public:
    GLuint mHandle {};
};