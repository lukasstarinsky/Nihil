#include "OpenGLMaterial.hpp"

OpenGLMaterial::OpenGLMaterial(const ShaderPtr& vertexShader, const ShaderPtr& fragmentShader)
    : mVertexShader{std::static_pointer_cast<OpenGLShader>(vertexShader)}
    , mFragmentShader{std::static_pointer_cast<OpenGLShader>(fragmentShader)}
{
    ASSERT(mVertexShader->GetType() == ShaderType::Vertex);
    ASSERT(mFragmentShader->GetType() == ShaderType::Fragment);

    GL_CHECK(mHandle = glCreateProgram());
    GL_CHECK(glAttachShader(mHandle, mVertexShader->GetHandle()));
    GL_CHECK(glAttachShader(mHandle, mFragmentShader->GetHandle()));
    GL_CHECK(glLinkProgram(mHandle));
}

OpenGLMaterial::~OpenGLMaterial()
{
    if (mHandle)
    {
        glDeleteProgram(mHandle);
    }
}

void OpenGLMaterial::Bind() const
{
    GL_CHECK(glUseProgram(mHandle));
}