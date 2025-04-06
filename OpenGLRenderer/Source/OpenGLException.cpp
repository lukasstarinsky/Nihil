#include "OpenGLException.hpp"

OpenGLException::OpenGLException(u32 error)
    : NihilException{"OpenGL", GlErrorToDescription(error), 2}
{

}

auto OpenGLException::GlErrorToDescription(u32 error) -> const char*
{
    switch (error)
    {
        case GL_INVALID_ENUM: return "Given when an enumeration parameter is not a legal enumeration for that function. This is given only for local problems; if the spec allows the enumeration in certain circumstances, where other parameters or state dictate those circumstances, then GL_INVALID_OPERATION is the result instead.";
        case GL_INVALID_VALUE: return "Given when a value parameter is not a legal value for that function. This is only given for local problems; if the spec allows the value in certain circumstances, where other parameters or state dictate those circumstances, then GL_INVALID_OPERATION is the result instead.";
        case GL_INVALID_OPERATION: return "Given when the set of state for a command is not legal for the parameters given to that command. It is also given for commands where combinations of parameters define what the legal parameters are.";
        case GL_STACK_OVERFLOW: return "Given when a stack pushing operation cannot be done because it would overflow the limit of that stack's size.";
        case GL_STACK_UNDERFLOW: return "Given when a stack popping operation cannot be done because the stack is already at its lowest point.";
        case GL_OUT_OF_MEMORY: return "Given when performing an operation that can allocate memory, and the memory cannot be allocated. The results of OpenGL functions that return this error are undefined; it is allowable for partial execution of an operation to happen in this circumstance.";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "Given when doing anything that would attempt to read from or write/render to a framebuffer that is not complete.";
        case GL_CONTEXT_LOST: return "Given if the OpenGL context has been lost, due to a graphics card reset.";
    }

    return "Unknown Error";
}