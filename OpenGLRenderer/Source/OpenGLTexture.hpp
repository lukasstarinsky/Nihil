#pragma once

#include "OpenGLCommon.hpp"

class OpenGLTexture : public Texture
{
public:
    OpenGLTexture(std::string_view filePath);
    ~OpenGLTexture() override;

    void Bind(i32 slot) const override;
private:
    GLuint mHandle {};
};