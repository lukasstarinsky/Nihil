#pragma once

#include "Core/Logger.hpp"

namespace Console
{
    void Print(std::string_view message, LogLevel severity);
}