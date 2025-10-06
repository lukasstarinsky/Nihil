#pragma once

#include "Platform/UUID.hpp"
#include "RendererAPI.hpp"

enum class ShaderStage : u32
{
    Vertex = 0,
    Fragment
};

struct ShaderVariant
{
    RendererAPI API;
    std::vector<std::byte> Data;
};

struct ShaderSpecification
{
    Nihil::UUID UUID {};
    ShaderStage Stage;
    std::vector<ShaderVariant> Variants;
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