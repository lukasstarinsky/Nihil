#include <fstream>
#include <filesystem>
#include "OpenGLShader.hpp"
#include "Renderer/ShaderCompiler.hpp"

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
    : mType{shaderType}
{
    std::vector<u32> spvBinary;

    std::filesystem::path spvFilePath = filePath + ".spv";
    if (std::filesystem::exists(spvFilePath))
    {
        std::ifstream file(spvFilePath, std::ios::binary | std::ios::ate);
        Ensure(file.is_open(), "Failed to open file: {}", spvFilePath.string());

        std::size_t size = file.tellg();
        file.seekg(0, std::ios::beg);
        spvBinary.resize(size / sizeof(u32));
        file.read(reinterpret_cast<char*>(spvBinary.data()), static_cast<std::streamsize>(size));
    }
    else
    {
        spvBinary = ShaderCompiler::GlslToSpv(filePath + ".glsl", shaderType);

        std::ofstream file(spvFilePath, std::ios::binary);
        Ensure(file.is_open(), "Failed to open file: {}", spvFilePath.string());
        file.write(reinterpret_cast<char*>(spvBinary.data()), static_cast<std::streamsize>(spvBinary.size() * sizeof(u32)));
    }

    GL_CHECK(mHandle = glCreateShader(ShaderTypeToGLenum(shaderType)));
    GL_CHECK(glShaderBinary(1, &mHandle, GL_SHADER_BINARY_FORMAT_SPIR_V, spvBinary.data(), static_cast<GLsizei>(sizeof(spvBinary[0]) * spvBinary.size())));
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

auto OpenGLShader::GetHandle() const -> GLuint
{
    return mHandle;
}

auto OpenGLShader::GetType() const -> ShaderType
{
    return mType;
}