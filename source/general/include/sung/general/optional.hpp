#pragma once

#include "os_detect.hpp"


#if defined(SUNG_CPP17)
    #include <optional>
#else
    #include <tl/optional.hpp>
#endif


namespace sung {

#if defined(SUNG_CPP17)
    template <class T>
    using Optional = std::optional<T>;

    static constexpr auto nullopt = std::nullopt;

    #pragma message("[SUNG] std::optional will be used")
#else
    template <class T>
    using Optional = tl::optional<T>;

    static constexpr auto nullopt = tl::nullopt;

    #pragma message("[SUNG] tl::optional will be used")
#endif

}
