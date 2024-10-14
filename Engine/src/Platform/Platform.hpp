#pragma once

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
        NASSERT(mFunctions.contains(name));
        return reinterpret_cast<T>(mFunctions[name]);
    }
private:
    std::string mName;
    void* mHandle;
    std::unordered_map<const char*, void*> mFunctions;
};

namespace Platform
{
    void Initialize(const ApplicationConfig& config);
    void Shutdown();
    void PollEvents();

    void* GetState();
}

namespace Console
{
    void Print(std::string_view message, LogLevel severity);
}