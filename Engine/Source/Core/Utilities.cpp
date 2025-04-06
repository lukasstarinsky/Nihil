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

auto File::ReadAll(const char* filePath) -> std::string
{
    std::ifstream file(filePath);
    Ensure(file.is_open(), "Failed to open file: {}", filePath);

    return {std::istreambuf_iterator<char>(file), {}};
}

auto File::ReadAllBytes(const char* filePath) -> std::vector<u8>
{
    std::ifstream file(filePath, std::ios::binary);
    Ensure(file.is_open(), "Failed to open file: {}", filePath);

    return {std::istream_iterator<u8>(file), {}};
}

auto File::ReadAllLines(const char* filePath) -> std::vector<std::string>
{
    std::ifstream file(filePath);
    Ensure(file.is_open(), "Failed to open file: {}", filePath);

    return {std::istream_iterator<std::string>(file), {}};
}

void File::WriteAllBytes(const char* filePath, const std::vector<u8>& bytes)
{
    std::ofstream file(filePath, std::ios::binary);
    Ensure(file.is_open(), "Failed to open file: {}", filePath);

    file.write(reinterpret_cast<const char*>(&bytes[0]), static_cast<std::streamsize>(bytes.size() * sizeof(u8)));
}