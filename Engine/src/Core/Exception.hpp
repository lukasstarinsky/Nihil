#pragma once

#include <exception>
#include <format>
#include "Defines.hpp"

class NihilException : public std::exception
{
public:
    NihilException(const char* file, u32 line, std::string_view kind, std::string_view message);
    NihilException(const char* file, u32 line, std::string_view message);

    const char* what() const noexcept override;
private:
    std::string mMessage;
};

#define NTHROW(message) throw NihilException(__FILE__, __LINE__, message)