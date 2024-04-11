#include <gtest/gtest.h>

#include "sung/general/aabb.hpp"
#include "sung/general/units.hpp"


namespace {

    TEST(AABB, WithDistance) {
        using Distance = sung::TDistance<float>;

        constexpr auto MIN_RANGE = Distance::from_nm(1);
        constexpr auto MAX_RANGE = Distance::from_nm(2);
        constexpr sung::AABB1<Distance> aabb(MIN_RANGE, MAX_RANGE);

        static_assert(
            aabb.is_inside_op(Distance::from_m(2000)), "AABB1 is_inside failed"
        );
        static_assert(
            !aabb.is_inside_op(Distance::from_m(1000)), "AABB1 is_inside failed"
        );
        static_assert(
            aabb.is_inside_cl(MAX_RANGE), "AABB1 is_contacting failed"
        );
        static_assert(!aabb.is_inside_op(MAX_RANGE), "AABB1 is_inside failed");

        static_assert(aabb.mid() == Distance::from_nm(1.5), "AABB1 mid failed");
        static_assert(
            aabb.len() == Distance::from_nm(1), "AABB1 length failed"
        );
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
