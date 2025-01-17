#include <gtest/gtest.h>

#include "sung/basic/units.hpp"
#include "sung/basic/mamath.hpp"


namespace {

    TEST(Units, DistanceConversions) {
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
    }


    TEST(Units, SpeedConversions) {
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
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
