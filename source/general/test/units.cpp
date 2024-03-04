#include "sung/general/mamath.hpp"
#include "sung/general/units.hpp"

#include <ratio>


namespace {

    int test_distance_conversions() {
        using Distance = sung::TDistance<float>;

        constexpr auto distance_m = Distance::from_metres(1000);
        constexpr auto distance_ft = Distance::from_feet(3280.84f);
        constexpr auto distance_miles = Distance::from_miles(0.621371f);
        constexpr auto distance_nautical_miles = Distance::from_nautical_miles(0.539957f);

        static_assert(sung::are_similiar(distance_m.feet(), distance_ft.feet(), 0.001f), "Distance conversion failed");
        static_assert(sung::are_similiar(distance_m.miles(), distance_miles.miles(), 0.001f), "Distance conversion failed");
        static_assert(sung::are_similiar(distance_m.nautical_miles(), distance_nautical_miles.nautical_miles(), 0.001f), "Distance conversion failed");

        return 0;
    }


    int test_speed_conversions() {
        using Speed = sung::TSpeed<float>;

        constexpr auto speed_ms = Speed::from_ms(100);
        constexpr auto speed_kts = Speed::from_kts(194.384f);
        constexpr auto speed_mph = Speed::from_mph(223.694f);

        static_assert(sung::are_similiar(speed_ms.kts(), speed_kts.kts(), 0.001f), "Speed conversion failed");
        static_assert(sung::are_similiar(speed_ms.mph(), speed_mph.mph(), 0.001f), "Speed conversion failed");

        return 0;
    }

}


int main() {
    if (0 != test_distance_conversions())
        return 1;
    if (0 != test_speed_conversions())
        return 2;

    return 0;
}
