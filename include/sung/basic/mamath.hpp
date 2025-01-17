#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>

#define SUNG_PI (3.14159265358979323846)
#define SUNG_TAU (SUNG_PI * 2)


namespace sung { namespace internal {

    template <typename T>
    constexpr int signum(T x, std::false_type is_signed) {
        constexpr auto ZERO = static_cast<T>(0);
        return ZERO < x;
    }

    template <typename T>
    constexpr int signum(T x, std::true_type is_signed) {
        constexpr auto ZERO = static_cast<T>(0);
        return (ZERO < x) - (x < ZERO);
    }


    template <typename T>
    T acos_safe_clamp(T x) {
        x = clamp(x, static_cast<T>(-1), static_cast<T>(1));
        return std::acos(x);
    }

    template <typename T>
    T acos_safe_branches(T x) {
        if (x < static_cast<T>(-1))
            return SUNG_PI;
        if (x > static_cast<T>(1))
            return 0;
        return std::acos(x);
    }

}}  // namespace sung::internal


namespace sung {

    template <typename T>
    constexpr T abs(const T x) {
        constexpr auto ZERO = static_cast<T>(0);
        return ZERO == x ? ZERO : (x < ZERO ? -x : x);
    }

    template <typename T>
    constexpr int signum(T x) {
        return internal::signum(x, std::is_signed<T>());
    }

    template <typename T>
    constexpr T floor(const T x) {
        constexpr auto ZERO = static_cast<T>(0);
        constexpr auto ONE = static_cast<T>(1);
        const auto x_int = static_cast<T>(static_cast<int64_t>(x));
        return (x >= ZERO ? x_int : (x == x_int ? x : x_int - ONE));
    }

    template <typename T>
    constexpr T clamp(T x, T min_value, T max_value) {
        return (std::max)(min_value, (std::min)(max_value, x));
    }

    template <typename T>
    constexpr bool are_similiar(T a, T b, T epsilon = 0) {
        // It must be <=, not < because the epsilon can be 0.
        return sung::abs(a - b) <= epsilon;
    }


    template <typename T>
    constexpr T to_degrees(T radians) {
        constexpr auto FACTOR = static_cast<T>(180.0 / SUNG_PI);
        return radians * FACTOR;
    }

    template <typename T>
    constexpr T to_radians(T degrees) {
        constexpr auto FACTOR = static_cast<T>(SUNG_PI / 180.0);
        return degrees * FACTOR;
    }


    template <typename T>
    T asin_safe(T x) {
        x = clamp(x, static_cast<T>(-1), static_cast<T>(1));
        return std::asin(x);
    }

    template <typename T>
    T acos_safe(T x) {
        return internal::acos_safe_branches(x);
    }


    /*
    @tparam T Floating poinrt number type.
    @return The linearly remapped value of `x` from the range [`min_bef`,
    `max_bef`] to the range [`min_aft`, `max_aft`].
    */
    template <typename T>
    constexpr T linear_remap(T x, T min_bef, T max_bef, T min_aft, T max_aft) {
        return (max_aft - min_aft) * (x - min_bef) / (max_bef - min_bef) +
               min_aft;
    }

    /*
    @tparam T Floating poinrt number type.
    @return 0 if `x` is less than `x_to_0`, 1 if `x` is greater than `x_to_1`,
    and a value between 0 and 1 if `x` is between `x_to_0` and `x_to_1`. The
    range is [0, 1].
    */
    template <typename T>
    constexpr T smoothstep_capped(T x, T x_to_0, T x_to_1) {
        auto t = (x - x_to_0) / (x_to_1 - x_to_0);
        t = sung::clamp<T>(t, (T)0, (T)1);
        return t * t * ((T)3 - (T)2 * t);
    }

}  // namespace sung
