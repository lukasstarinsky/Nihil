#pragma once

/* ======= Macros ======= */
#define COUNT_OF(x) sizeof(x) / sizeof(x[0])
#define TO_STR_(x) #x
#define TO_STR(x) TO_STR_(x)

#define KILOBYTE(x) ((x) * 1024)
#define MEGABYTE(x) KILOBYTE(1024 * x)
#define GIGABYTE(x) MEGABYTE(1024 * x)
#define TERABYTE(x) GIGABYTE(1024 * x)

#define UNUSED [[maybe_unused]]

/* ======= Platform Detection ======= */
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #ifdef _WIN64
        #define NIHIL_PLATFORM_WINDOWS
        #define DEBUGBREAK() __debugbreak()
    #else
        #error "Platform not supported."
    #endif // _WIN64
#elif defined(__APPLE__)
    #define NIHIL_PLATFORM_APPLE
    #error "Platform not supported."
#elif defined(__linux__)
    #define NIHIL_PLATFORM_LINUX
    #include <csignal>
    #define DEBUGBREAK() raise(SIGTRAP)
    #error "Platform not supported."
#else
    #error "Unknown Platform"
#endif

/* ======= API Macros ======= */
#ifdef NIHIL_EXPORT
    #ifdef NIHIL_PLATFORM_WINDOWS
        #define NIHIL_API __declspec(dllexport)
    #else
        #define NIHIL_API __attribute__((visibility("default")))
    #endif
#else
    #ifdef NIHIL_PLATFORM_WINDOWS
        #define NIHIL_API __declspec(dllimport)
    #else
        #define NIHIL_API
    #endif
#endif

/* ======= Asserts ======= */
#ifdef NDEBUG
    #define ASSERT(check, ...)
#else
    #define ASSERT(check, ...) do { if(!(check)) { Logger::Fatal("Assertion failed [" __FILE__ ":" TO_STR(__LINE__) "] " __VA_ARGS__); DEBUGBREAK(); } } while(0)
#endif

/* ======= Type Defs ======= */
#include <cstdint>

using i8  = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using f32 = float;
using f64 = double;

static_assert(sizeof(f32) == 4, "Float is expected to be 4 bytes.");
static_assert(sizeof(f64) == 8, "Double is expected to be 8 bytes.");


/* ======= Concepts ======= */
#include <concepts>

template <typename T, typename... U>
concept IsAnyOf = (std::same_as<T, U> || ...);