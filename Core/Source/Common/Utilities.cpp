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

auto File::Read(const std::filesystem::path& filePath) -> std::string
{
    std::ifstream file(filePath);
    Ensure(file.is_open(), "Failed to open file: {}", filePath.string());

    return {std::istreambuf_iterator<char>(file), {}};
}

auto File::ReadLines(const std::filesystem::path& filePath) -> std::vector<std::string>
{
    std::ifstream file(filePath);
    Ensure(file.is_open(), "Failed to open file: {}", filePath.string());

    std::vector<std::string> out;
    std::string line;
    while (std::getline(file, line))
    {
        out.push_back(line);
    }
    return out;
}

void File::WriteBinary(std::string_view filePath, const void* data, std::size_t size)
{
    std::ofstream file(filePath.data(), std::ios::binary);
    Ensure(file.is_open(), "Failed to open file: {}", filePath.data());
    Ensure(!!file.write(reinterpret_cast<const char*>(data), static_cast<std::streamsize>(size)), "Failed to write to binary file: {}", filePath);
}