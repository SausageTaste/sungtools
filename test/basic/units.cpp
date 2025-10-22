#include <gtest/gtest.h>

#include "sung/basic/mamath.hpp"
#include "sung/basic/units.hpp"


namespace {

    TEST(Units, DistanceConversions) {
        using Distance = sung::TLength<float>;

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


    TEST(Units, TemperatureConversions) {
        using Temperature = sung::TTemperature<double>;

        constexpr auto temp_k = Temperature::from_k(300);
        constexpr auto temp_c = Temperature::from_c(26.85);
        constexpr auto temp_f = Temperature::from_f(80.33);

        static_assert(
            sung::are_similiar(temp_k.c(), temp_c.c(), 0.0001),
            "Temperature conversion failed"
        );
        static_assert(
            sung::are_similiar(temp_k.f(), temp_f.f(), 0.0001),
            "Temperature conversion failed"
        );
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
