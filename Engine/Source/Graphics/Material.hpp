#pragma once

#include "DefaultResource.hpp"
#include "Math/Math.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

struct MaterialSpecification
{
    Nihil::UUID VertexShaderUUID = DefaultResource::VertexShader;
    Nihil::UUID FragmentShaderUUID = DefaultResource::FragmentShader;
    Nihil::UUID TextureUUID = DefaultResource::Texture;
};

struct MaterialCreateInfo
{
    ShaderPtr VertexShader;
    ShaderPtr FragmentShader;
    TexturePtr Texture;
};

class Material;
using MaterialPtr = std::shared_ptr<Material>;

class Material
{
public:
    virtual ~Material() = default;

    virtual void Bind() const = 0;
    virtual void SetUniform(i32 location, const Mat4f& data) const = 0;

    static auto Create(const MaterialCreateInfo& materialCreateInfo) -> MaterialPtr;
};