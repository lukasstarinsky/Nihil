#include <fstream>
#include <chrono>

#include "Utilities.hpp"

auto Time::GetFormattedTime(bool includeDate) -> std::string
{
    using namespace std::chrono;

    auto now = time_point_cast<seconds>(current_zone()->to_local(system_clock::now()));

    if (includeDate)
        return std::format("{:%D %T}", now);
    else
        return std::format("{:%T}", now);
}

auto File::Read(std::string_view filePath) -> std::string
{
    std::ifstream file(filePath.data());
    Ensure(file.is_open(), "Failed to open file: {}", filePath.data());

    return {std::istreambuf_iterator<char>(file), {}};
}

auto File::ReadLines(std::string_view filePath) -> std::vector<std::string>
{
    std::ifstream file(filePath.data());
    Ensure(file.is_open(), "Failed to open file: {}", filePath.data());

    return {std::istream_iterator<std::string>(file), {}};
}

void File::WriteBinary(std::string_view filePath, const void* data, std::size_t size)
{
    std::ofstream file(filePath.data(), std::ios::binary);
    Ensure(file.is_open(), "Failed to open file: {}", filePath.data());
    Ensure(!!file.write(reinterpret_cast<const char*>(data), static_cast<std::streamsize>(size)), "Failed to write to binary file: {}", filePath);
}