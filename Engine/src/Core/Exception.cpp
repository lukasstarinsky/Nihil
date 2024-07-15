#include "Exception.hpp"

NihilException::NihilException(u32 line, const char* file, std::string_view message)
{
    mMessage = std::format("Exception thrown in [{}:{}]: {}", file, line, message);
}

const char* NihilException::what() const noexcept
{
    return mMessage.c_str();
}