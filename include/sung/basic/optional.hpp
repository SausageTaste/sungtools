#pragma once

#if defined(SUNG_FORCE_OPTIONAL_TL)
    #define SUNG_OPTIONAL_TL
#elif defined(SUNG_FORCE_OPTIONAL_STD)
    #define SUNG_OPTIONAL_STD
#else
    #ifdef __cpp_lib_optional
        #define SUNG_OPTIONAL_STD
    #else
        #define SUNG_OPTIONAL_TL
    #endif
#endif

#if defined(SUNG_OPTIONAL_STD)
    #include <optional>
#elif defined(SUNG_OPTIONAL_TL)
    #include <tl/optional.hpp>
#else
    #error "Unknown optional implementation"
#endif

#include "sung/basic/os_detect.hpp"


namespace sung {

#if defined(SUNG_OPTIONAL_STD)
    template <class T>
    using Optional = std::optional<T>;

    using bad_optional_access = std::bad_optional_access;

    static constexpr auto nullopt = std::nullopt;

    template <typename T>
    SUNG_NODISCARD auto make_optional(T&& value) {
        return std::make_optional(std::forward<T>(value));
    }

#elif defined(SUNG_OPTIONAL_TL)
    template <class T>
    using Optional = tl::optional<T>;

    using bad_optional_access = tl::bad_optional_access;

    static constexpr auto nullopt = tl::nullopt;

    template <typename T>
    SUNG_NODISCARD auto make_optional(T&& value) {
        return tl::make_optional(std::forward<T>(value));
    }

#endif

    template <typename T>
    using Opt = Optional<T>;

}  // namespace sung
