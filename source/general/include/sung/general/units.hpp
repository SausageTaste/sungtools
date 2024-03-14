#pragma once

#include "ratio.hpp"


namespace sung {

    template <typename T>
    class TDistance {

    public:
        constexpr static TDistance from_metres(T metres) {
            return TDistance(metres);
        }
        constexpr void set_metres(T metres) { metres_ = metres; }
        constexpr T    metres() const { return metres_; }

        constexpr static TDistance from_feet(T feet) {
            return TDistance(feet * METRES_PER_FOOT.value<T>());
        }
        constexpr void set_feet(T feet) {
            metres_ = feet * METRES_PER_FOOT.value<T>();
        }
        constexpr T feet() const {
            return metres_ * METRES_PER_FOOT.reciprocal<T>();
        }

        constexpr static TDistance from_miles(T miles) {
            return TDistance(miles * METRES_PER_MILE.value<T>());
        }
        constexpr void set_miles(T miles) {
            metres_ = miles * METRES_PER_MILE.value<T>();
        }
        constexpr T miles() const {
            return metres_ * METRES_PER_MILE.reciprocal<T>();
        }

        constexpr static TDistance from_nautical_miles(T nm) {
            return TDistance(nm * METRES_PER_NAUTICAL_MILE.value<T>());
        }
        constexpr void set_nautical_miles(T nm) {
            metres_ = nm * METRES_PER_NAUTICAL_MILE.value<T>();
        }
        constexpr T nautical_miles() const {
            return metres_ * METRES_PER_NAUTICAL_MILE.reciprocal<T>();
        }

        constexpr TDistance() = default;

        constexpr TDistance operator+(const TDistance& rhs) const {
            return TDistance(metres_ + rhs.metres_);
        }
        constexpr TDistance operator-(const TDistance& rhs) const {
            return TDistance(metres_ - rhs.metres_);
        }

        constexpr bool operator<(const TDistance& rhs) const {
            return metres_ < rhs.metres_;
        }
        constexpr bool operator>(const TDistance& rhs) const {
            return metres_ > rhs.metres_;
        }
        constexpr bool operator<=(const TDistance& rhs) const {
            return metres_ <= rhs.metres_;
        }
        constexpr bool operator>=(const TDistance& rhs) const {
            return metres_ >= rhs.metres_;
        }
        constexpr bool operator==(const TDistance& rhs) const {
            return metres_ == rhs.metres_;
        }
        constexpr bool operator!=(const TDistance& rhs) const {
            return metres_ != rhs.metres_;
        }

        // These are exact values
        constexpr static Ratio<int> METRES_PER_FOOT{ 3048, 10000 };
        constexpr static Ratio<int> METRES_PER_MILE{ 1609344, 1000 };
        constexpr static Ratio<int> METRES_PER_NAUTICAL_MILE{ 1852 };

    private:
        constexpr explicit TDistance(T metres) : metres_(metres) {}

        T metres_ = 0;
    };


    template <typename T>
    class TSpeed {

    public:
        // Metres per second
        constexpr static TSpeed from_ms(T value) { return TSpeed(value); }
        constexpr void          set_ms(T value) { metres_per_second_ = value; }
        constexpr T             ms() const { return metres_per_second_; }

        // Knots (nautical miles per hour)
        constexpr static TSpeed from_kts(T value) {
            return TSpeed<T>(value * MS_PER_KTS.template value<T>());
        }
        constexpr void set_kts(T value) {
            metres_per_second_ = value * MS_PER_KTS.template value<T>();
        }
        constexpr T kts() const {
            return metres_per_second_ * MS_PER_KTS.template reciprocal<T>();
        }

        // Miles per hour
        constexpr static TSpeed from_mph(T value) {
            return TSpeed<T>(value * (MS_PER_MPH.template value<T>)());
        }
        constexpr void set_mph(T value) {
            metres_per_second_ = value * (MS_PER_MPH.template value<T>)();
        }
        constexpr T mph() const {
            return metres_per_second_ * MS_PER_MPH.template reciprocal<T>();
        }

        constexpr TSpeed() = default;

        constexpr TSpeed operator+(const TSpeed& rhs) const {
            return TSpeed(metres_per_second_ + rhs.metres_per_second_);
        }
        constexpr TSpeed operator-(const TSpeed& rhs) const {
            return TSpeed(metres_per_second_ - rhs.metres_per_second_);
        }
        constexpr TSpeed operator*(T rhs) const {
            return TSpeed(metres_per_second_ * rhs);
        }
        constexpr TSpeed operator/(T rhs) const {
            return TSpeed(metres_per_second_ / rhs);
        }

        constexpr bool operator<(const TSpeed& rhs) const {
            return metres_per_second_ < rhs.metres_per_second_;
        }
        constexpr bool operator>(const TSpeed& rhs) const {
            return metres_per_second_ > rhs.metres_per_second_;
        }
        constexpr bool operator<=(const TSpeed& rhs) const {
            return metres_per_second_ <= rhs.metres_per_second_;
        }
        constexpr bool operator>=(const TSpeed& rhs) const {
            return metres_per_second_ >= rhs.metres_per_second_;
        }
        constexpr bool operator==(const TSpeed& rhs) const {
            return metres_per_second_ == rhs.metres_per_second_;
        }
        constexpr bool operator!=(const TSpeed& rhs) const {
            return metres_per_second_ != rhs.metres_per_second_;
        }

        constexpr static Ratio<int> SEC_PER_HOUR{ 3600 };

        constexpr static auto MS_PER_KTS =
            TDistance<T>::METRES_PER_NAUTICAL_MILE / SEC_PER_HOUR;
        constexpr static auto MS_PER_MPH =
            TDistance<T>::METRES_PER_MILE / SEC_PER_HOUR;

    private:
        constexpr explicit TSpeed(T value) : metres_per_second_(value) {}

        T metres_per_second_ = 0;
    };

}  // namespace sung
