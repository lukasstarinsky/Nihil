#pragma once

enum class ShaderType
{
    Vertex = 0,
    Fragment
};

class Shader;
using ShaderPtr = std::shared_ptr<Shader>;

class Shader
{
public:
    virtual ~Shader() = default;

    static auto Create(const std::string& filePath, ShaderType shaderType) -> ShaderPtr;
};