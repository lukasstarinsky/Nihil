#include "OpenGLTexture.hpp"

#include <stb/stb_image.h>

OpenGLTexture::OpenGLTexture(std::string_view filePath)
{
    // TODO: pass the raw file data, dont load here
    i32 width, height, numChannels;
    auto data = stbi_load(filePath.data(), &width, &height, &numChannels, 0);
    Ensure(data, "Failed to load image file: {}", filePath);

    glCreateTextures(GL_TEXTURE_2D, 1, &mHandle);

    glTextureParameteri(mHandle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(mHandle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(mHandle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(mHandle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureStorage2D(mHandle, 1, GL_RGBA8, width, height);
    glTextureSubImage2D(mHandle, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
}

OpenGLTexture::~OpenGLTexture()
{
    glDeleteTextures(0, &mHandle);
}

void OpenGLTexture::Bind(i32 slot) const
{
    glBindTextureUnit(slot, mHandle);
}