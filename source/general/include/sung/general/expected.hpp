#pragma once

#if defined(__cpp_lib_expected)
    #include <expected>
#else
    #include "tl/expected.hpp"
#endif


namespace sung {

#if defined(__cpp_lib_expected)

    #pragma message("[SUNG] std::expected will be used")

    template <typename T, typename E>
    using Expected = std::expected<T, E>;

    using std::unexpected;

#else

    #pragma message("[SUNG] tl::expected will be used")

    template <typename T, typename E>
    using Expected = tl::expected<T, E>;

    using tl::unexpected;

#endif

}
