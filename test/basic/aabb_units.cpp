#include <gtest/gtest.h>

#include "sung/basic/aabb.hpp"
#include "sung/basic/units.hpp"


namespace {

    TEST(AABB, WithDistance) {
        using Dist = sung::TLength<float>;

        constexpr auto MIN_RANGE = Dist::from_nm(1);
        constexpr auto MAX_RANGE = Dist::from_nm(2);
        constexpr sung::Aabb1D<Dist> aabb(MIN_RANGE, MAX_RANGE);

        static_assert(aabb.is_inside_op(Dist::from_m(2000)), "");
        static_assert(!aabb.is_inside_op(Dist::from_m(1000)), "");
        static_assert(aabb.is_inside_cl(MAX_RANGE), "");
        static_assert(!aabb.is_inside_op(MAX_RANGE), "");

        static_assert(aabb.mid() == Dist::from_nm(1.5), "");
        static_assert(aabb.len() == Dist::from_nm(1), "");
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
