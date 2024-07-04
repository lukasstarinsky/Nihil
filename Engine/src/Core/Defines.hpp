#pragma once

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