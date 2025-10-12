#pragma once

#include "DefaultResource.hpp"
#include "Math/Math.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Resource.hpp"

struct MaterialSpecification
{
    Nihil::UUID UUID = DefaultResource::ObjectMaterial;
    Nihil::UUID VertexShaderUUID = DefaultResource::ObjectVertexShader;
    Nihil::UUID FragmentShaderUUID = DefaultResource::ObjectFragmentShader;
};

struct MaterialCreateInfo
{
    ShaderPtr VertexShader;
    ShaderPtr FragmentShader;
};

class Material;
using MaterialPtr = std::shared_ptr<Material>;

class Material : public Resource
{
public:
    using Specification = MaterialSpecification;

    virtual ~Material() = default;

    virtual void Bind() const = 0;
    virtual void SetUniform(i32 location, const Mat4f& data) const = 0;

    static auto Create(const MaterialCreateInfo& createInfo) -> MaterialPtr;
};