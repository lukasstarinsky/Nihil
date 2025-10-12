#include "OpenGLMaterial.hpp"

OpenGLMaterial::OpenGLMaterial(const MaterialCreateInfo& createInfo)
    : mVertexShader{std::static_pointer_cast<OpenGLShader>(createInfo.VertexShader)}
    , mFragmentShader{std::static_pointer_cast<OpenGLShader>(createInfo.FragmentShader)}
{
    ASSERT(mVertexShader->GetStage() == ShaderStage::Vertex);
    ASSERT(mFragmentShader->GetStage() == ShaderStage::Fragment);

    mHandle = glCreateProgram();
    glAttachShader(mHandle, mVertexShader->GetHandle());
    glAttachShader(mHandle, mFragmentShader->GetHandle());
    glLinkProgram(mHandle);
}

OpenGLMaterial::~OpenGLMaterial()
{
    glDeleteProgram(mHandle);
}

void OpenGLMaterial::Bind() const
{
    glUseProgram(mHandle);
}

void OpenGLMaterial::SetUniform(i32 location, const Mat4f& data) const
{
    Bind();
    glUniformMatrix4fv(location, 1, GL_FALSE, data.Data());
    glUseProgram(0);
}