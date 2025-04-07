#pragma once

#include <string>
#include "Defines.hpp"

namespace Time
{
    auto NIHIL_API GetFormattedTime(bool includeDate = false) -> std::string;
}

namespace File
{
    auto NIHIL_API ReadAll(std::string_view filePath) -> std::string;
    auto NIHIL_API ReadAllLines(std::string_view filePath) -> std::vector<std::string>;
}