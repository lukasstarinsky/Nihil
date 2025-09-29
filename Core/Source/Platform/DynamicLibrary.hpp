#pragma once

#include <unordered_map>

class NIHIL_API DynamicLibrary
{
public:
    static auto Load(const char* name, DynamicLibrary* outDynamicLibrary) -> bool;

    auto Unload() const -> bool;
    auto LoadSymbol(const char* name) -> bool;

    template<typename T>
    auto GetSymbol(const char* name) -> T
    {
        ASSERT(mSymbols.contains(name));
        return reinterpret_cast<T>(mSymbols[name]);
    }
private:
    std::string mName;
    void* mHandle {};
    std::unordered_map<const char*, void*> mSymbols;
};
