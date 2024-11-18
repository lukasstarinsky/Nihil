#pragma once

class DynamicLibrary
{
public:
    static auto Load(const char* name, DynamicLibrary* outDynamicLibrary) -> bool;

    auto Unload() const -> bool;
    auto LoadFunction(const char* name) -> bool;

    template<typename T>
    auto GetFunction(const char* name) -> T
    {
        ASSERT(mFunctions.contains(name));
        return reinterpret_cast<T>(mFunctions[name]);
    }
private:
    std::string mName;
    void* mHandle {};
    std::unordered_map<const char*, void*> mFunctions;
};
