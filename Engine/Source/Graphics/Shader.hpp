#pragma once

#include "Resource.hpp"
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

using ShaderCreateInfo = ShaderSpecification;

class Shader;
using ShaderPtr = std::shared_ptr<Shader>;

class Shader : public Resource
{
public:
    using Specification = ShaderSpecification;

    virtual ~Shader() = default;
public:
    static auto Create(const ShaderCreateInfo& createInfo) -> ShaderPtr;
};