#include "OpenGLMaterial.hpp"

OpenGLMaterial::OpenGLMaterial(const ShaderPtr& vertexShader, const ShaderPtr& fragmentShader)
    : mVertexShader{std::static_pointer_cast<OpenGLShader>(vertexShader)}
    , mFragmentShader{std::static_pointer_cast<OpenGLShader>(fragmentShader)}
{
    ASSERT(mVertexShader->GetType() == ShaderType::Vertex);
    ASSERT(mFragmentShader->GetType() == ShaderType::Fragment);

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