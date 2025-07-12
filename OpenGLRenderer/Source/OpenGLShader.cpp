#include "OpenGLShader.hpp"

#include <fstream>
#include <filesystem>
#include "Graphics/ShaderCompiler.hpp"

static auto ShaderTypeToGLenum(ShaderStage shaderType) -> GLenum
{
    switch (shaderType)
    {
        case ShaderStage::Vertex: return GL_VERTEX_SHADER;
        case ShaderStage::Fragment: return GL_FRAGMENT_SHADER;
    }

    return GL_VERTEX_SHADER;
}

OpenGLShader::OpenGLShader(const ShaderSpecification& shaderSpec)
    : mShaderStage{shaderSpec.Stage}
{
    mHandle = glCreateShader(ShaderTypeToGLenum(mShaderStage));
    glShaderBinary(1, &mHandle, GL_SHADER_BINARY_FORMAT_SPIR_V, shaderSpec.Data.data(), static_cast<GLsizei>(sizeof(shaderSpec.Data[0]) * shaderSpec.Data.size()));
    glSpecializeShader(mHandle, "main", 0, nullptr, nullptr);

    i32 compiled;
    glGetShaderiv(mHandle, GL_COMPILE_STATUS, &compiled);
    Ensure(compiled, "Failed to specialize shader");
}

OpenGLShader::~OpenGLShader()
{
    glDeleteShader(mHandle);
}

auto OpenGLShader::GetHandle() const -> GLuint
{
    return mHandle;
}

auto OpenGLShader::GetStage() const -> ShaderStage
{
    return mShaderStage;
}