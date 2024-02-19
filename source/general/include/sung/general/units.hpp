#pragma once


namespace sung {

    template <typename T>
    class TDistance {

    public:
        constexpr static TDistance from_metres(T metres) {
            return TDistance(metres);
        }
        constexpr static TDistance from_feet(T feet) {
            return TDistance(feet * METRES_PER_FEET);
        }
        constexpr static TDistance from_miles(T miles) {
            return TDistance(miles * METRES_PER_MILES);
        }
        constexpr static TDistance from_nautical_miles(T nautical_miles) {
            return TDistance(nautical_miles * METRES_PER_NAUTICAL_MILE);
        }

        constexpr TDistance operator+(const TDistance& rhs) const {
            return TDistance(metres_ + rhs.metres_);
        }
        constexpr TDistance operator-(const TDistance& rhs) const {
            return TDistance(metres_ - rhs.metres_);
        }

        constexpr T metres() const {
            return metres_;
        }
        constexpr T feet() const {
            constexpr auto FACTOR = 1.0 / METRES_PER_FEET;
            return metres_ * FACTOR;
        }
        constexpr T miles() const {
            constexpr auto FACTOR = 1.0 / METRES_PER_MILES;
            return metres_ * FACTOR;
        }
        constexpr T nautical_miles() const {
            constexpr auto FACTOR = 1.0 / METRES_PER_NAUTICAL_MILE;
            return metres_ * FACTOR;
        }

        // These are exact values
        constexpr static T METRES_PER_FEET = 0.3048;
        constexpr static T METRES_PER_MILES = 1609.344;
        constexpr static T METRES_PER_NAUTICAL_MILE = 1852;

    private:
        constexpr explicit TDistance(T metres) : metres_(metres) {}

        T metres_ = 0;

    };


    template <typename T>
    class TSpeed {

    public:
        constexpr static TSpeed from_ms(T value) {
            return TSpeed(value);
        }
        constexpr static TSpeed from_kts(T value) {
            return TSpeed(value * MS_PER_KTS);
        }

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

        constexpr T ms() const {
            return metres_per_second_;
        }
        constexpr T kts() const {
            constexpr T FACTOR = 1.0 / MS_PER_KTS;
            return metres_per_second_ * FACTOR;
        }

        constexpr static T SEC_PER_HOUR = 3600;
        constexpr static T MS_PER_KTS = TDistance<T>::METRES_PER_NAUTICAL_MILE / SEC_PER_HOUR;

    private:
        constexpr explicit TSpeed(T value)
            : metres_per_second_(value)
        {}

        T metres_per_second_ = 0;

    };

}
