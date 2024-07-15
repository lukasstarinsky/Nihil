#include "Exception.hpp"

NihilException::NihilException(u32 line, const char* file, const char* type, std::string_view message)
{
    mMessage = std::format("{} thrown in [{}:{}]: {}", type, file, line, message);
}

const char* NihilException::what() const noexcept
{
    return mMessage.c_str();
}