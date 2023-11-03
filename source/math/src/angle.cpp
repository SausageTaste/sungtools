#include "sung/math/angle.hpp"

#include <cmath>


namespace {

    constexpr double DEG_TO_RAD = sung::PI / 180.0;
    constexpr double RAD_TO_DEG = 180.0 / sung::PI;


    // [0, 2pi)
    inline double positive_radians(const double x) {
        constexpr double FACTOR = sung::PI * 2.0;
        constexpr double FACTOR_INV = 1.0 / FACTOR;
        return x - std::floor(x * FACTOR_INV) * FACTOR;
    }

    // [-pi, pi)
    inline double negative_radians(const double x) {
        constexpr double FACTOR = sung::PI * 2.0;
        constexpr double FACTOR_INV = 1.0 / FACTOR;
        return x - std::floor(x * FACTOR_INV + 0.5) * FACTOR;
    }

}


namespace sung {

    Angle Angle::from_deg(double degrees) {
        return Angle{ degrees * DEG_TO_RAD };
    }

    Angle Angle::operator+(const Angle& rhs) const {
        return Angle{ radians_ + rhs.radians_ };
    }

    Angle Angle::operator-(const Angle& rhs) const {
        return Angle{ radians_ - rhs.radians_ };
    }

    Angle Angle::operator*(double rhs) const {
        return Angle{ radians_ * rhs };
    }

    Angle Angle::operator/(double rhs) const {
        return Angle{ radians_ / rhs };
    }

    bool Angle::is_equivalent(const Angle& rhs, double epsilon) const {
        const auto lhs_rad = ::negative_radians(radians_);
        const auto rhs_rad = ::negative_radians(rhs.radians_);
        return std::abs(lhs_rad - rhs_rad) <= epsilon;
    }

    double Angle::deg() const {
        return radians_ * RAD_TO_DEG;
    }

    void Angle::set_deg(double degrees) {
        radians_ = degrees * DEG_TO_RAD;
    }

    Angle Angle::normalize_pos() const {
        return Angle{ ::positive_radians(radians_) };
    }

    Angle Angle::normalize_neg() const {
        return Angle{ ::negative_radians(radians_) };
    }

    Angle Angle::calc_short_diff(Angle rhs) const {
        constexpr double MAX_VALUE = PI * 2.0;
        const auto da = std::fmod(rhs.radians_ - radians_, MAX_VALUE);
        return Angle{ std::fmod(da * 2.0, MAX_VALUE) - da };
    }

    Angle Angle::lerp(Angle rhs, double t) const {
        return (*this) + this->calc_short_diff(rhs) * t;
    }

}
