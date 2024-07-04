#pragma once

#include <vector>
#include "Core/Application.hpp"
#include "Core/Logger.hpp"

struct DynamicLibrary
{
    std::string Name;
    void* Handle;
    std::vector<void*> Functions;
};

namespace Platform
{
    bool Initialize(const ApplicationConfig& config);
    void Shutdown();
    void PollEvents();

    NIHIL_API bool LoadDynamicLibrary(const char* name, DynamicLibrary* outDynamicLibrary);
    NIHIL_API bool UnloadDynamicLibrary(const DynamicLibrary& dynamicLibrary);
    NIHIL_API bool LoadDynamicLibraryFunction(DynamicLibrary& dynamicLibrary, const char* funName);
}

namespace Console
{
    void Print(std::string_view message, LogLevel severity);
}