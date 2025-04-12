#pragma once

class Texture;
using TexturePtr = std::shared_ptr<Texture>;

class NIHIL_API Texture
{
public:
    virtual ~Texture() = default;

    virtual void Bind(i32 slot) const = 0;

    static auto Create(std::string_view filePath) -> TexturePtr;
};