#pragma once

#include "Core/Exception.hpp"

class OpenGLException : public NihilException
{
public:
    explicit OpenGLException(u32 error);
private:
    static auto GlErrorToDescription(u32 error) -> const char*;
};

#define GL_THROW(error) throw OpenGLException(error)