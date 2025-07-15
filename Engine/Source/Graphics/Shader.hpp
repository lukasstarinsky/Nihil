#pragma once

enum class ShaderStage
{
    Vertex = 0,
    Fragment
};

struct ShaderSpecification
{
    std::string Name;
    ShaderStage Stage;
    std::vector<std::byte> Data;
};

class Shader;
using ShaderPtr = std::shared_ptr<Shader>;

class Shader
{
public:
    virtual ~Shader() = default;
public:
    static auto Create(const ShaderSpecification& shaderSpec) -> ShaderPtr;
};