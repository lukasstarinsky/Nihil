#include <chrono>
#include "Utilities.hpp"

std::string Utilities::GetFormattedTime(bool includeDate)
{
    using namespace std::chrono;

    auto now { time_point_cast<seconds>(current_zone()->to_local(system_clock::now())) };

    if (includeDate)
        return std::format("{:%D %T}", now);
    else
        return std::format("{:%T}", now);
}
