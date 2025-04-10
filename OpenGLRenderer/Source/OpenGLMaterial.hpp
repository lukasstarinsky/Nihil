#pragma once

#include "OpenGLCommon.hpp"
#include "OpenGLShader.hpp"

class OpenGLMaterial : public Material
{
public:
    OpenGLMaterial(const ShaderPtr& vertexShader, const ShaderPtr& fragmentShader);
    ~OpenGLMaterial() override;

    void Bind() const override;

    void SetUniform(i32 location, const Mat4f& data) const override;
private:
    GLuint mHandle {};
    std::shared_ptr<OpenGLShader> mVertexShader {};
    std::shared_ptr<OpenGLShader> mFragmentShader {};
};