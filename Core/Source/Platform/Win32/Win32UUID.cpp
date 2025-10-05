#include "Platform/UUID.hpp"

#include <rpc.h>

Nihil::UUID::UUID()
{
    ::UUID uuid;
    UuidCreate(&uuid);
    std::memcpy(mData, &uuid, sizeof(::UUID));
}

bool Nihil::UUID::operator==(const UUID& other) const
{
    return std::memcmp(mData, other.mData, 16) == 0;
}

auto Nihil::UUID::Hash() const -> size_t
{
    RPC_STATUS status;
    auto hash = UuidHash(reinterpret_cast<::UUID*>(mData), &status);
    Ensure(status == RPC_S_OK, "Failed to hash UUID.");
    return hash;
}

auto Nihil::UUID::ToString() const -> std::string
{
    RPC_CSTR str;
    UuidToString(reinterpret_cast<::UUID*>(mData), &str);
    std::string uuidStr(reinterpret_cast<char*>(str));
    RpcStringFree(&str);
    return uuidStr;
}

auto Nihil::UUID::FromString(const std::string& str) -> UUID
{
    UUID uuid;
    Ensure(UuidFromString(reinterpret_cast<RPC_CSTR>(const_cast<char*>(str.c_str())), reinterpret_cast<::UUID*>(uuid.mData)) == RPC_S_OK, "Invalid UUID string: {}", str);
    return uuid;
}