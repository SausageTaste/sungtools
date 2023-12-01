#pragma once

#include <cmath>


namespace sung {

    /*
    Strong type for angles
    This class eliminates the confusion between radians and degrees.

    Just be careful and choose correct unit when using `from_*`, `set_*` functions.
    For instance, calling `set_deg` with radians parameter will cause unexpected result.

    The class uses radians internally.
    So using any methods that deals with radians is recommended.
    By that you can avoid unnecessary conversion and precision problem.

    Comparing two angles are not strictly defined.
    For instance, 0 deg and 360 deg can be treated as a same angle, but sometimes not.
    So, you may use `normalize_*` function to normalize the angles before comparing.
    Or you can call `rad` to get numeric value and compare them.
    */
    template <typename T>
    class TAngle {

    public:
        TAngle() = default;

        static TAngle from_deg(T degrees) { return TAngle{ degrees * DEG_TO_RAD }; }
        static TAngle from_rad(T radians) { return TAngle{ radians }; }

        TAngle operator+(const TAngle& rhs) const { return TAngle{ radians_ + rhs.radians_ }; }
        TAngle operator-(const TAngle& rhs) const { return TAngle{ radians_ - rhs.radians_ }; }

        // Multiply an angle by a scalar, not an angle.
        // Product of two angles are usually not an angle.
        // You can use `rad` or `deg` functions to get numeric value to use it in your folmulas.
        TAngle operator*(T rhs) const { return TAngle{ radians_ * rhs }; }
        TAngle operator/(T rhs) const { return TAngle{ radians_ / rhs }; }

        bool is_equivalent(const TAngle& rhs, T epsilon = 0) const {
            const auto diff = std::abs(this->rad_diff(radians_, rhs.radians_));
            return diff <= epsilon;
        }

        T deg() const { return radians_ * RAD_TO_DEG; }
        T rad() const { return radians_; }

        void set_deg(T degrees) { radians_ = degrees * DEG_TO_RAD; }
        void set_rad(T radians) { radians_ = radians; }

        // Normalize to [0, 2pi), retaining the phase
        TAngle normalize_pos() const { return TAngle{ this->positive_radians(radians_) }; }
        // Normalize to [-pi, pi), retaining the phase
        TAngle normalize_neg() const { return TAngle{ this->negative_radians(radians_) }; }

        // https://gist.github.com/shaunlebron/8832585
        // Calculate the shortest angular distance from this to rhs
        TAngle calc_short_diff(TAngle rhs) const {
            return TAngle(this->rad_diff(radians_, rhs.radians_));
        }
        TAngle lerp(TAngle rhs, T t) const {
            return (*this) + this->calc_short_diff(rhs) * t;
        }

    private:
        explicit TAngle(T radians) : radians_(radians) {}

        // [0, 2pi)
        static T positive_radians(T x) {
            return x - std::floor(x * PI2_INV) * PI2;
        }

        // [-pi, pi)
        static T negative_radians(T x) {
            return x - std::floor(x * PI2_INV + static_cast<T>(0.5)) * PI2;
        }

        static T rad_diff(T a, T b) {
            const auto da = std::fmod(b - a, PI2);
            return std::fmod(da * static_cast<T>(2), PI2) - da;
        }

        constexpr static T PI = 3.14159265358979323846;
        constexpr static T PI2 = PI * static_cast<T>(2);
        constexpr static T PI2_INV = static_cast<T>(1) / PI2;

        constexpr static T DEG_TO_RAD = PI / 180.0;
        constexpr static T RAD_TO_DEG = 180.0 / PI;

        T radians_ = 0;

    };


    using Angle = TAngle<double>;

}
