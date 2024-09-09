#pragma once

#if defined(__cpp_lib_optional)
    #include <optional>
#else
    #include <tl/optional.hpp>
#endif


namespace sung {

#if defined(__cpp_lib_optional)
    template <class T>
    using Optional = std::optional<T>;

    using bad_optional_access = std::bad_optional_access;

    static constexpr auto nullopt = std::nullopt;

    template <typename T>
    [[nodiscard]] auto make_optional(T&& value) {
        return std::make_optional(std::forward<T>(value));
    }

#else
    template <class T>
    using Optional = tl::optional<T>;

    using bad_optional_access = tl::bad_optional_access;

    static constexpr auto nullopt = tl::nullopt;

    template <typename T>
    [[nodiscard]] auto make_optional(T&& value) {
        return tl::make_optional(std::forward<T>(value));
    }

#endif

}  // namespace sung
