#pragma once

#include <vector>
#include <unordered_map>
#include "Core/Application.hpp"
#include "Core/Logger.hpp"

class DynamicLibrary
{
public:
    static bool Load(const char* name, DynamicLibrary* outDynamicLibrary);

    bool Unload() const;
    bool LoadFunction(const char* name);

    template<typename T>
    T GetFunction(const char* name)
    {
        NASSERT(Functions.contains(name));
        return reinterpret_cast<T>(Functions[name]);
    }
public:
    std::string Name;
    void* Handle;
    std::unordered_map<std::string, void*> Functions;
};

namespace Platform
{
    void Initialize(const ApplicationConfig& config);
    void Shutdown();
    void PollEvents();

    NIHIL_API void* GetState();
}

namespace Console
{
    void Print(std::string_view message, LogLevel severity);
}