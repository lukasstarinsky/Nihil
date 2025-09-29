#include "Exception.hpp"

NihilException::NihilException(std::string_view message, u32 stackTraceSkip)
    : mMessage{std::format("Exception thrown: {}", message)}
    , mStackTrace{std::stacktrace::current(stackTraceSkip)}
{

}

NihilException::NihilException(std::string_view kind, std::string_view message, u32 stackTraceSkip)
    : mMessage{std::format("{} Exception thrown: {}", kind, message)}
    , mStackTrace{std::stacktrace::current(stackTraceSkip)}
{

}

auto NihilException::what() const noexcept -> const char*
{
    return mMessage.c_str();
}

auto NihilException::StackTrace() const -> std::string
{
    return std::to_string(mStackTrace);
}