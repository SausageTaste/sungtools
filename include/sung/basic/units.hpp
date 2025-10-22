#pragma once

#include "ratio.hpp"


namespace sung {

    template <typename T>
    class TLength {

    public:
        // Metres
        constexpr static TLength from_m(T metres) { return TLength(metres); }
        constexpr void set_m(T metres) { metres_ = metres; }
        constexpr T m() const { return metres_; }

        // Kilometres
        constexpr static TLength from_km(T kilometres) {
            return TLength(kilometres * M_PER_KM.value<T>());
        }
        constexpr void set_km(T kilometres) {
            metres_ = kilometres * M_PER_KM.value<T>();
        }
        constexpr T km() const { return metres_ * M_PER_KM.reciprocal<T>(); }

        // Feet
        constexpr static TLength from_ft(T feet) {
            return TLength(feet * M_PER_FT.value<T>());
        }
        constexpr void set_ft(T feet) { metres_ = feet * M_PER_FT.value<T>(); }
        constexpr T ft() const { return metres_ * M_PER_FT.reciprocal<T>(); }

        // Miles
        constexpr static TLength from_mi(T miles) {
            return TLength(miles * M_PER_MI.value<T>());
        }
        constexpr void set_mi(T miles) {
            metres_ = miles * M_PER_MI.value<T>();
        }
        constexpr T mi() const { return metres_ * M_PER_MI.reciprocal<T>(); }

        // Nautical miles
        constexpr static TLength from_nm(T nm) {
            return TLength(nm * M_PER_NM.value<T>());
        }
        constexpr void set_nm(T nm) { metres_ = nm * M_PER_NM.value<T>(); }
        constexpr T nm() const { return metres_ * M_PER_NM.reciprocal<T>(); }

        constexpr TLength() = default;

        constexpr TLength operator+(const TLength& rhs) const {
            return TLength(metres_ + rhs.metres_);
        }
        constexpr TLength operator-(const TLength& rhs) const {
            return TLength(metres_ - rhs.metres_);
        }
        constexpr TLength operator*(T rhs) const {
            return TLength(metres_ * rhs);
        }
        constexpr TLength operator/(T rhs) const {
            return TLength(metres_ / rhs);
        }

        constexpr bool operator<(const TLength& rhs) const {
            return metres_ < rhs.metres_;
        }
        constexpr bool operator>(const TLength& rhs) const {
            return metres_ > rhs.metres_;
        }
        constexpr bool operator<=(const TLength& rhs) const {
            return metres_ <= rhs.metres_;
        }
        constexpr bool operator>=(const TLength& rhs) const {
            return metres_ >= rhs.metres_;
        }
        constexpr bool operator==(const TLength& rhs) const {
            return metres_ == rhs.metres_;
        }
        constexpr bool operator!=(const TLength& rhs) const {
            return metres_ != rhs.metres_;
        }

        // These are exact values
        // metres per kilometre
        constexpr static Ratio<int> M_PER_KM{ 1000 };
        // metres per foot
        constexpr static Ratio<int> M_PER_FT{ 3048, 10000 };
        // metres per mile
        constexpr static Ratio<int> M_PER_MI{ 1609344, 1000 };
        // metres per nautical mile
        constexpr static Ratio<int> M_PER_NM{ 1852 };

    private:
        constexpr explicit TLength(T metres) : metres_(metres) {}

        T metres_ = 0;
    };


    template <typename T>
    class TSpeed {

    public:
        // Metres per second
        constexpr static TSpeed from_ms(T value) { return TSpeed(value); }
        constexpr void set_ms(T value) { metres_per_second_ = value; }
        constexpr T ms() const { return metres_per_second_; }

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

        // Seconds per hour
        constexpr static Ratio<int> SEC_PER_H{ 3600 };

        constexpr static auto MS_PER_KTS = TLength<T>::M_PER_NM / SEC_PER_H;
        constexpr static auto MS_PER_MPH = TLength<T>::M_PER_MI / SEC_PER_H;

    private:
        constexpr explicit TSpeed(T value) : metres_per_second_(value) {}

        T metres_per_second_ = 0;
    };

}  // namespace sung
