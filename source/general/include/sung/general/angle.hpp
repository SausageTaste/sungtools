#pragma once

#include "mamath.hpp"


namespace sung {

    // Normalize to [0, 2pi), retaining the phase.
    // Consider some angles like 512 degrees, which can be simplified to 152
    // degrees. 360 degrees equals to 0 degrees, -42 degrees equals to 318
    // degrees, etc... This function does that. Note that I'm using degrees for
    // explanation, but the function uses radians internally.
    /// @tparam T Either `float` or `double`
    /// @param x Angle in radians
    /// @return Normalized angle in radians in range [0, 2pi)
    template <typename T>
    constexpr T repeat_rad_positive(T x) {
        constexpr auto TAU = static_cast<T>(SUNG_TAU);
        constexpr auto TAU_INV = static_cast<T>(1.0 / (SUNG_TAU));
        return x - sung::floor(x * TAU_INV) * TAU;
    }


    // Normalize to [-pi, pi), retaining the phase.
    // Same notion as `repeat_rad_positive`, but it maps values to [-pi, pi), or
    // [-180, 180) in degrees. 512 degrees will be 152 degrees, which is
    // identical to result of `repeat_rad_positive`. But some angles like 272
    // degrees will be -88 degrees to make it fit in [-180, 180).
    //
    // You don't want to use this function for comparing two angles' similarity.
    // Because -179 degrees and 179 degrees are very similar, but numerically
    // they are very different. So, you may want to use `calc_rad_shortest_diff`
    // function instead, which will output -2 degrees for the case above.
    /// @tparam T Either `float` or `double`
    /// @param x Angle in radians
    /// @return Normalized angle in radians in range [-pi, pi)
    template <typename T>
    constexpr T repeat_rad_negative(T x) {
        constexpr auto TAU = static_cast<T>(SUNG_TAU);
        constexpr auto TAU_INV = static_cast<T>(1.0 / (SUNG_TAU));
        return x - sung::floor(x * TAU_INV + static_cast<T>(0.5)) * TAU;
    }


    template <typename T>
    T calc_rad_shortest_diff_mod(T from, T to) {
        constexpr auto TAU = static_cast<T>(SUNG_TAU);
        const auto da = std::fmod(to - from, TAU);
        return std::fmod(da * static_cast<T>(2), TAU) - da;
    }

    template <typename T>
    constexpr T calc_rad_shortest_diff_floor(T from, T to) {
        return repeat_rad_negative(to - from);
    }


    // Calculate the shortest angular distance from `a` to `b`.
    // The result will be in [-pi, pi).
    // For instance, if `a` is 0 degrees and `b` is 270 degrees, the result will
    // be -90 degrees. If `a` is 178 degrees and `b` is -169 degrees, the
    // function will magically find the shortest path and outputs -13 degrees,
    // which can be added to `a` and get new angle whose phase equals to `rhs`.
    // That means `repeat_rad_negative(a + new angle) =
    // repeat_rad_negative(rhs)`, ignoring the float precision problem. Check
    // out https://gist.github.com/shaunlebron/8832585 for more details. Also
    // there is a simple interactive Desmos graph
    // (https://www.desmos.com/calculator/bkrjdpk2am) you can play with.
    /// @tparam T Either `float` or `double`
    /// @param from Start angle in radians
    /// @param to Destination angle in radians
    /// @return Shortest angular distance in radians
    template <typename T>
    constexpr T calc_rad_shortest_diff(T from, T to) {
        return calc_rad_shortest_diff_floor<T>(from, to);
    }


    /*
    Strong type for angles.
    This class eliminates the confusion between radians and degrees.

    Just be careful and choose correct unit when using `from_*`, `set_*`
    functions. For instance, calling `set_deg` with radians parameter will cause
    unexpected result.

    The class uses radians internally.
    So using any methods that deals with radians is recommended.
    By that you can avoid unnecessary conversion and precision problem.

    Comparing two angles are not strictly defined.
    For instance, 0 deg and 360 deg can be treated as a same angle, but
    sometimes not. So, you may use `normalize_*` function to normalize the
    angles before comparing. Or you can call `rad` to get numeric value and
    compare them.

    @tparam T Either `float` or `double`
    */
    template <typename T>
    class TAngle {

    public:
        constexpr TAngle() = default;

        constexpr static TAngle from_deg(T degrees) {
            return TAngle{ sung::to_radians(degrees) };
        }
        constexpr static TAngle from_rad(T radians) {
            return TAngle{ radians };
        }
        constexpr static TAngle from_zero() {
            return TAngle{ 0 };
        }  // You can just use default ctor but for explicitness

        constexpr TAngle operator+(const TAngle& rhs) const {
            return TAngle{ radians_ + rhs.radians_ };
        }
        constexpr TAngle operator-(const TAngle& rhs) const {
            return TAngle{ radians_ - rhs.radians_ };
        }

        // Multiply an angle by a scalar, not an angle.
        // Product of two angles are usually not an angle.
        // You can use `rad` or `deg` functions to get numeric value to use it
        // in your folmulas.
        constexpr TAngle operator*(T rhs) const {
            return TAngle{ radians_ * rhs };
        }
        constexpr TAngle operator/(T rhs) const {
            return TAngle{ radians_ / rhs };
        }

        constexpr bool is_equivalent(const TAngle& rhs, T epsilon = 0) const {
            const auto diff =
                sung::calc_rad_shortest_diff(radians_, rhs.radians_);
            // It must be <=, not < because the epsilon can be 0.
            return sung::abs(diff) <= epsilon;
        }

        constexpr T deg() const { return sung::to_degrees(radians_); }
        constexpr T rad() const { return radians_; }

        constexpr void set_deg(T degrees) {
            radians_ = sung::to_radians(degrees);
        }
        constexpr void set_rad(T radians) { radians_ = radians; }
        constexpr void set_zero() { radians_ = 0; }

        constexpr TAngle add_deg(T degrees) const {
            return TAngle{ radians_ + sung::to_radians(degrees) };
        }
        constexpr TAngle add_rad(T radians) {
            return TAngle{ radians_ + radians };
        }

        constexpr TAngle normalize_pos() const {
            return TAngle{ sung::repeat_rad_positive(radians_) };
        }
        constexpr TAngle normalize_neg() const {
            return TAngle{ sung::repeat_rad_negative(radians_) };
        }

        constexpr TAngle calc_short_diff_to(TAngle dst) const {
            return TAngle(sung::calc_rad_shortest_diff(radians_, dst.radians_));
        }
        constexpr TAngle calc_short_diff_from(TAngle start) const {
            return TAngle(sung::calc_rad_shortest_diff(start.radians_, radians_)
            );
        }
        constexpr TAngle lerp(TAngle dst, T t) const {
            return (*this) + this->calc_short_diff_to(dst) * t;
        }

    private:
        constexpr explicit TAngle(T radians) : radians_(radians) {}

        T radians_ = 0;
    };


    static_assert(
        sizeof(TAngle<float>) == sizeof(float),
        "TAngle<float> must be as big as underlying type"
    );
    static_assert(
        sizeof(TAngle<double>) == sizeof(double),
        "TAngle<double> must be as big as underlying type"
    );

}  // namespace sung
