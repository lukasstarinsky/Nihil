#include "Platform/UUID.hpp"

#include <rpc.h>

Nihil::UUID::UUID(std::byte data[16])
{
    std::memcpy(mData, data, sizeof(mData));
}

Nihil::UUID::UUID(i32 num)
{
    std::memset(mData, num, sizeof(mData));
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

auto Nihil::UUID::Generate() -> UUID
{
    Nihil::UUID nUUID;
    ::UUID uuid;
    UuidCreate(&uuid);
    std::memcpy(nUUID.mData, &uuid, sizeof(::UUID));
    return nUUID;
}