#include <fstream>
#include <filesystem>
#include "OpenGLShader.hpp"
#include "Graphics/ShaderCompiler.hpp"

static auto ShaderTypeToGLenum(ShaderType shaderType) -> GLenum
{
    switch (shaderType)
    {
        case ShaderType::Vertex: return GL_VERTEX_SHADER;
        case ShaderType::Fragment: return GL_FRAGMENT_SHADER;
    }

    return GL_VERTEX_SHADER;
}

OpenGLShader::OpenGLShader(const std::string& filePath, ShaderType shaderType)
    : mShaderType{shaderType}
{
    std::vector<u32> spvBinary;

    std::string spvFilePath = filePath + ".spv";
    if (std::filesystem::exists(spvFilePath))
    {
        spvBinary = File::ReadBinary<u32>(spvFilePath);
    }
    else
    {
        spvBinary = ShaderCompiler::GlslToSpv(filePath + ".glsl", shaderType);
        File::WriteBinary(spvFilePath, spvBinary.data(), spvBinary.size() * sizeof(u32));
    }

    mHandle = glCreateShader(ShaderTypeToGLenum(shaderType));
    glShaderBinary(1, &mHandle, GL_SHADER_BINARY_FORMAT_SPIR_V, spvBinary.data(), static_cast<GLsizei>(sizeof(spvBinary[0]) * spvBinary.size()));
    glSpecializeShader(mHandle, "main", 0, nullptr, nullptr);

    i32 compiled;
    glGetShaderiv(mHandle, GL_COMPILE_STATUS, &compiled);
    Ensure(compiled, "Failed to specialize shader: {}", filePath);
}

OpenGLShader::~OpenGLShader()
{
    glDeleteShader(mHandle);
}

auto OpenGLShader::GetHandle() const -> GLuint
{
    return mHandle;
}

auto OpenGLShader::GetType() const -> ShaderType
{
    return mShaderType;
}