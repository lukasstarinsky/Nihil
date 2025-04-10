#pragma once

#include "Shader.hpp"

class Material;
using MaterialPtr = std::shared_ptr<Material>;

class Material
{
public:
    virtual ~Material() = default;

    virtual void Bind() const = 0;

    static auto Create(const ShaderPtr& vertexShader, const ShaderPtr& fragmentShader) -> MaterialPtr;
};