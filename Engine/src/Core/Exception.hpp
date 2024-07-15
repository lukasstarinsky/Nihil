#pragma once

#include <exception>
#include <format>
#include "Defines.hpp"

class NIHIL_API NihilException : public std::exception
{
public:
    NihilException(u32 line, const char* file, const char* type, std::string_view message);

    const char* what() const noexcept override;
private:
    std::string mMessage;
};

#define NTHROW(message)             throw NihilException(__LINE__, __FILE__, "Exception", message)
#define NTHROW_TYPE(type, message)  throw NihilException(__LINE__, __FILE__, type, message)