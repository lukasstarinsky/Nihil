#pragma once

#include "Common/Defines.hpp"

namespace Nihil
{

class NIHIL_API UUID
{
public:
    UUID() = default;
    UUID(std::byte data[16]);
    UUID(i32 num);

    bool operator==(const UUID& other) const;
    auto Hash() const -> size_t;
    auto ToString() const -> std::string;

    static auto FromString(const std::string& str) -> UUID;
    static auto Generate() -> UUID;
private:
    mutable std::byte mData[16]{};

    friend struct std::hash<UUID>;
    friend struct std::formatter<UUID>;
};

}

template<>
struct std::hash<Nihil::UUID>
{
    size_t operator()(const Nihil::UUID& id) const noexcept
    {
        return id.Hash();
    }
};

template <>
class std::formatter<Nihil::UUID>
{
public:
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    template <typename Context>
    auto format(const Nihil::UUID& uuid, Context& ctx) const
    {
        return std::format_to(ctx.out(), "{}", uuid.ToString());
    }
};
