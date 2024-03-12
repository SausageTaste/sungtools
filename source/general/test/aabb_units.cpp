#include "sung/general/aabb.hpp"
#include "sung/general/units.hpp"


namespace {

    template <typename T>
    int test_distance_aabb1() {
        using Distance = sung::TDistance<T>;

        constexpr auto MIN_RANGE = Distance::from_nautical_miles(1);
        constexpr auto MAX_RANGE = Distance::from_nautical_miles(2);
        constexpr sung::AABB1<Distance> aabb(MIN_RANGE, MAX_RANGE);

        static_assert(aabb.is_inside_op(Distance::from_metres(2000)), "AABB1 is_inside failed");
        static_assert(!aabb.is_inside_op(Distance::from_metres(1000)), "AABB1 is_inside failed");
        static_assert(aabb.is_inside_cl(MAX_RANGE), "AABB1 is_contacting failed");
        static_assert(!aabb.is_inside_op(MAX_RANGE), "AABB1 is_inside failed");
        static_assert(aabb.len() == Distance::from_nautical_miles(1), "AABB1 length failed");

        return 0;
    }

}


int main() {
    if (0 != test_distance_aabb1<double>())
        return 1;
    if (0 != test_distance_aabb1<float>())
        return 1;

    return 0;
}
