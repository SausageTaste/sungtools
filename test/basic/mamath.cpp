#include "sung/basic/mamath.hpp"

#include <gtest/gtest.h>


namespace {

    TEST(Mamath, Signum) {
        ASSERT_LT(sung::signum(-45), 0);
        ASSERT_EQ(sung::signum(0), 0);
        ASSERT_GT(sung::signum(45), 0);

        ASSERT_GT(sung::signum(-45U), 0);
        ASSERT_EQ(sung::signum(0U), 0);
        ASSERT_GT(sung::signum(45U), 0);

        ASSERT_LT(sung::signum(-45.0), 0);
        ASSERT_EQ(sung::signum(0.0), 0);
        ASSERT_GT(sung::signum(45.0), 0);
    }


    TEST(Mamath, LinearRemap) {
        using T = double;

        ASSERT_DOUBLE_EQ(sung::linear_remap<T>(0, 0, 10, 0, 100), 0);
        ASSERT_DOUBLE_EQ(sung::linear_remap<T>(5, 0, 10, 0, 100), 50);
        ASSERT_DOUBLE_EQ(sung::linear_remap<T>(10, 0, 10, 0, 100), 100);
    }


    TEST(Mamath, SmoothstepCapped) {
        using T = double;

        ASSERT_DOUBLE_EQ(sung::smoothstep_capped<T>(10, 0, 10), 1);
        ASSERT_DOUBLE_EQ(sung::smoothstep_capped<T>(5, 0, 10), 0.5);
        ASSERT_DOUBLE_EQ(sung::smoothstep_capped<T>(10, 0, 100), 0.028);
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
