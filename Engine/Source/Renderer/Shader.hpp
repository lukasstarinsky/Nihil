#pragma once

enum class ShaderType
{
    Vertex = 0,
    Fragment
};

class Shader
{
public:
    virtual ~Shader() = default;

    static auto Create(std::string_view filePath, ShaderType shaderType) -> std::shared_ptr<Shader>;
};