#pragma once

#include <string>
#include "Defines.hpp"

namespace Utilities
{
    NIHIL_API auto GetFormattedTime(bool includeDate = false) -> std::string;
}