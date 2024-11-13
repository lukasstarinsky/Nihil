#pragma once

#include <exception>
#include <stacktrace>

class NIHIL_API NihilException : public std::exception
{
public:
    explicit NihilException(std::string_view message);
    NihilException(std::string_view kind, std::string_view message);

    auto what() const noexcept -> const char* override;
    auto StackTrace() const -> std::string;
private:
    std::string mMessage;
    std::stacktrace mStackTrace;
};

#define THROW(message) throw NihilException(message)