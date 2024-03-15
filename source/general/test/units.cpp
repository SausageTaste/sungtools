#include "sung/general/units.hpp"
#include "sung/general/mamath.hpp"


namespace {

    int test_distance_conversions() {
        using Distance = sung::TDistance<float>;

        constexpr auto distance_m = Distance::from_m(1000);
        constexpr auto distance_ft = Distance::from_ft(3280.84f);
        constexpr auto distance_miles = Distance::from_mi(0.621371f);
        constexpr auto distance_nautical_miles = Distance::from_nm(0.539957f);

        static_assert(
            sung::are_similiar(distance_m.ft(), distance_ft.ft(), 0.001f),
            "Distance conversion failed"
        );
        static_assert(
            sung::are_similiar(distance_m.mi(), distance_miles.mi(), 0.001f),
            "Distance conversion failed"
        );
        static_assert(
            sung::are_similiar(
                distance_m.nm(), distance_nautical_miles.nm(), 0.001f
            ),
            "Distance conversion failed"
        );

        return 0;
    }


    int test_speed_conversions() {
        using Speed = sung::TSpeed<float>;

        constexpr auto speed_ms = Speed::from_ms(100);
        constexpr auto speed_kts = Speed::from_kts(194.384f);
        constexpr auto speed_mph = Speed::from_mph(223.694f);

        static_assert(
            sung::are_similiar(speed_ms.kts(), speed_kts.kts(), 0.001f),
            "Speed conversion failed"
        );
        static_assert(
            sung::are_similiar(speed_ms.mph(), speed_mph.mph(), 0.001f),
            "Speed conversion failed"
        );

        return 0;
    }

}  // namespace


int main() {
    if (0 != test_distance_conversions())
        return 1;
    if (0 != test_speed_conversions())
        return 2;

    return 0;
}
