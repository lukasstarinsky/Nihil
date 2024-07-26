#include "Exception.hpp"

NihilException::NihilException(const char* file, u32 line, std::string_view message)
{
    mMessage = std::format("Exception thrown in [{}:{}]: {}", file, line, message);
}

NihilException::NihilException(const char* file, u32 line, std::string_view kind, std::string_view message)
{
    mMessage = std::format("{} Exception thrown in [{}:{}]: {}", kind, file, line, message);
}

const char* NihilException::what() const noexcept
{
    return mMessage.c_str();
}