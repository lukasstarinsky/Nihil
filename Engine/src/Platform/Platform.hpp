#pragma once

#include <unordered_map>
#include "Core/Application.hpp"
#include "Core/Logger.hpp"

class DynamicLibrary
{
public:
    static auto Load(const char* name, DynamicLibrary* outDynamicLibrary) -> bool;

    auto Unload() const -> bool;
    auto LoadFunction(const char* name) -> bool;

    template<typename T>
    auto GetFunction(const char* name) -> T
    {
        NASSERT(mFunctions.contains(name));
        return reinterpret_cast<T>(mFunctions[name]);
    }
private:
    std::string mName;
    void* mHandle {};
    std::unordered_map<const char*, void*> mFunctions;
};

namespace Platform
{
    void Initialize(const ApplicationConfig& config);
    void Shutdown();
    void PollEvents();
    auto GetState() -> void*;
}

namespace Console
{
    void Print(std::string_view message, LogLevel severity);
}