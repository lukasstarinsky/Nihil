#pragma once

#include <vector>

#include "Platform/UUID.hpp"

struct TextureSpecification
{
    Nihil::UUID UUID {};
    i32 Width, Height;
    std::vector<std::byte> Data;
};

class Texture;
using TexturePtr = std::shared_ptr<Texture>;

class Texture
{
public:
    virtual ~Texture() = default;

    virtual void Bind(i32 slot) const = 0;
public:
    static auto Create(const TextureSpecification& textureSpec) -> TexturePtr;
};