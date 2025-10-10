#include "OpenGLMaterial.hpp"

OpenGLMaterial::OpenGLMaterial(const MaterialCreateInfo& materialCreateInfo)
    : mVertexShader{std::static_pointer_cast<OpenGLShader>(materialCreateInfo.VertexShader)}
    , mFragmentShader{std::static_pointer_cast<OpenGLShader>(materialCreateInfo.FragmentShader)}
    , mTexture{std::static_pointer_cast<OpenGLTexture>(materialCreateInfo.Texture)}
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
    if (mTexture)
        mTexture->Bind(0);
}

void OpenGLMaterial::SetUniform(i32 location, const Mat4f& data) const
{
    Bind();
    glUniformMatrix4fv(location, 1, GL_FALSE, data.Data());
    glUseProgram(0);
}