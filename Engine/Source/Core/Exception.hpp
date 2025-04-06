#pragma once

#include <exception>
#include <stacktrace>

#define THROW(message) throw NihilException(message)

class NIHIL_API NihilException : public std::exception
{
public:
    explicit NihilException(std::string_view message, u32 stackTraceSkip = 1);
    NihilException(std::string_view kind, std::string_view message, u32 stackTraceSkip = 1);

    auto what() const noexcept -> const char* override;
    auto StackTrace() const -> std::string;
private:
    std::string mMessage;
    std::stacktrace mStackTrace;
};

template <>
class std::formatter<NihilException>
{
public:
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    template <typename Context>
    auto format(const NihilException& e, Context& ctx) const
    {
        return std::format_to(ctx.out(), "{}\nStack Trace:\n{}", e.what(), e.StackTrace());
    }
};

template <typename... Args>
inline void Ensure(bool predicate, std::format_string<Args...> msg, Args&&... args)
{
    if (!predicate)
    {
        throw NihilException(std::format(msg, std::forward<Args>(args)...), 2);
    }
}