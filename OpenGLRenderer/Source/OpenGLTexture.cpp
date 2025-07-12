#include "OpenGLTexture.hpp"

OpenGLTexture::OpenGLTexture(const TextureSpecification& textureSpec)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &mHandle);

    glTextureParameteri(mHandle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(mHandle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(mHandle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(mHandle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureStorage2D(mHandle, 1, GL_RGBA8, textureSpec.Width, textureSpec.Height);
    glTextureSubImage2D(mHandle, 0, 0, 0, textureSpec.Width, textureSpec.Height, GL_RGBA, GL_UNSIGNED_BYTE, textureSpec.Data.data());
}

OpenGLTexture::~OpenGLTexture()
{
    glDeleteTextures(0, &mHandle);
}

void OpenGLTexture::Bind(i32 slot) const
{
    glBindTextureUnit(slot, mHandle);
}