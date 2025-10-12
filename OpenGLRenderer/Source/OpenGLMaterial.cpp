#include "OpenGLMaterial.hpp"

OpenGLMaterial::OpenGLMaterial(const MaterialCreateInfo& createInfo)
    : Material{createInfo}
    , mVertexShader{std::static_pointer_cast<OpenGLShader>(createInfo.VertexShader)}
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