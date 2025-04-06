#include "OpenGLShader.hpp"
#include "Renderer/ShaderCompiler.hpp"

static auto ShaderTypeToGLEnum(ShaderType shaderType) -> GLenum
{
    switch (shaderType)
    {
        case ShaderType::Vertex: return GL_VERTEX_SHADER;
        case ShaderType::Fragment: return GL_FRAGMENT_SHADER;
    }

    return GL_VERTEX_SHADER;
}

OpenGLShader::OpenGLShader(std::string_view filePath, ShaderType shaderType)
{
    auto sourceCode = File::ReadAll(filePath.data());
    auto spvBinary = ShaderCompiler::GlslToSpv(sourceCode, shaderType);

    GL_CHECK(mHandle = glCreateShader(ShaderTypeToGLEnum(shaderType)));
    GL_CHECK(glShaderBinary(1, &mHandle, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, spvBinary.data(), static_cast<GLsizei>(sizeof(spvBinary[0]) * spvBinary.size())));
    GL_CHECK(glSpecializeShader(mHandle, "main", 0, nullptr, nullptr));

    i32 compiled;
    glGetShaderiv(mHandle, GL_COMPILE_STATUS, &compiled);
    Ensure(compiled, "Failed to specialize shader: {}", filePath);
}

OpenGLShader::~OpenGLShader()
{
    if (mHandle)
    {
        glDeleteShader(mHandle);
    }
}