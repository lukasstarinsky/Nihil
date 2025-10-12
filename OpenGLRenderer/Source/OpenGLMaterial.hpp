#pragma once

#include "OpenGLCommon.hpp"
#include "OpenGLShader.hpp"
#include "OpenGLTexture.hpp"

class OpenGLMaterial : public Material
{
public:
    explicit OpenGLMaterial(const MaterialCreateInfo& createInfo);
    ~OpenGLMaterial() override;

    void Bind() const override;
private:
    GLuint mHandle {};
    std::shared_ptr<OpenGLShader> mVertexShader {};
    std::shared_ptr<OpenGLShader> mFragmentShader {};
};