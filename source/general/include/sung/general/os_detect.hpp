#pragma once

#include <cstdint>


// https://stackoverflow.com/questions/5919996/how-to-detect-reliably-mac-os-x-ios-linux-windows-in-c-preprocessor
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define SUNG_OS_WINDOWS

    #ifdef _WIN64
        #define SUNG_OS_WINDOWS_X64
    #else
        #define SUNG_OS_WINDOWS_X32
    #endif
#elif __ANDROID__
    #define SUNG_OS_ANDROID
#elif __APPLE__
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR
         // iOS Simulator
    #elif TARGET_OS_IPHONE
        // iOS device
    #elif TARGET_OS_MAC
        // Other kinds of Mac OS
    #else
        #error "Unknown Apple platform"
    #endif
#elif __linux__
    #define SUNG_OS_LINUX
#elif __unix__ // all unices not caught above
    #define SUNG_OS_UNIX
#elif defined(_POSIX_VERSION)
    // POSIX
#else
    #error "Unknown compiler"
#endif


// https://stackoverflow.com/questions/5272825/detecting-64bit-compile-in-c
#if INTPTR_MAX == INT64_MAX
    #define SUNG_SYS_X64
#elif INTPTR_MAX == INT32_MAX
    #define SUNG_SYS_X32
#else
    #error Unknown pointer size or missing size macros!
#endif


#if defined(_MSVC_LANG)
    #define SUNG__cplusplus _MSVC_LANG
#else
    #define SUNG__cplusplus __cplusplus
#endif

#if SUNG__cplusplus >= 202002L
    #define SUNG_CPP20
#elif SUNG__cplusplus >= 201703L
    #define SUNG_CPP17
#elif SUNG__cplusplus >= 201402L
    #define SUNG_CPP14
#elif SUNG__cplusplus >= 201103L
    #define SUNG_CPP11
#endif
