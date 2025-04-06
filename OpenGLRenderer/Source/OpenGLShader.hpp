#pragma once

#include <string_view>
#include "OpenGLCommon.hpp"

class OpenGLShader : public Shader
{
public:
    OpenGLShader(std::string_view filePath, ShaderType shaderType);
    ~OpenGLShader() override;
private:
    GLuint mHandle {};
};