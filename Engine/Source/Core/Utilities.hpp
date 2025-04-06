#pragma once

#include <string>
#include "Defines.hpp"

namespace Time
{
    NIHIL_API auto GetFormattedTime(bool includeDate = false) -> std::string;
}

namespace File
{
    NIHIL_API auto ReadAll(const char* filePath) -> std::string;
    NIHIL_API auto ReadAllBytes(const char* filePath) -> std::vector<u8>;
    NIHIL_API auto ReadAllLines(const char* filePath) -> std::vector<std::string>;

    NIHIL_API void WriteAllBytes(const char* filePath, const std::vector<u8>& bytes);
}