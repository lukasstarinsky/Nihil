#pragma once

#include "Shader.hpp"

class Material;
using MaterialPtr = std::shared_ptr<Material>;

class Material
{
public:
    virtual ~Material() = default;

    virtual void Bind() const = 0;

    // TODO: will this be here?
    virtual void SetUniform(i32 location, const Mat4f& data) const = 0;

    static auto Create(const ShaderPtr& vertexShader, const ShaderPtr& fragmentShader) -> MaterialPtr;
};