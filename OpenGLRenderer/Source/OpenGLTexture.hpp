#pragma once

#include "OpenGLCommon.hpp"

class OpenGLTexture : public Texture
{
public:
    OpenGLTexture(const TextureSpecification& textureSpec);
    ~OpenGLTexture() override;

    void Bind(i32 slot) const override;
private:
    GLuint mHandle {};
};