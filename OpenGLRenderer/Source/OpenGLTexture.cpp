#include "OpenGLTexture.hpp"

OpenGLTexture::OpenGLTexture(const TextureCreateInfo& textureCreateInfo)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &mHandle);

    glTextureParameteri(mHandle, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(mHandle, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(mHandle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(mHandle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureStorage2D(mHandle, 1, GL_RGBA8, textureCreateInfo.Width, textureCreateInfo.Height);
    glTextureSubImage2D(mHandle, 0, 0, 0, textureCreateInfo.Width, textureCreateInfo.Height, GL_RGBA, GL_UNSIGNED_BYTE, textureCreateInfo.Data.data());
}

OpenGLTexture::~OpenGLTexture()
{
    glDeleteTextures(0, &mHandle);
}

void OpenGLTexture::Bind(i32 slot) const
{
    glBindTextureUnit(slot, mHandle);
}

auto OpenGLTexture::GetHandle() const -> GLuint
{
    return mHandle;
}