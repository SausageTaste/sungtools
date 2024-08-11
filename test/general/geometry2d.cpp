#include "sung/general/geometry2d.hpp"

#include <vector>

#include <gtest/gtest.h>

#include "sung/general/random.hpp"


namespace {

    TEST(Geometry3D, Triangle2) {
        using Tri = sung::Triangle2<double>;
        sung::RandomRealNumGenerator<double> rng{ -1000, 1000 };

        {
            const Tri tri{ { 0, 0 }, { 1, 0 }, { 0, 1 } };

            ASSERT_TRUE(tri.is_inside_cl({ 0.5, 0.5 }));
            ASSERT_TRUE(tri.is_inside_cl({ 0.2, 0.2 }));
            ASSERT_TRUE(tri.is_inside_cl({ 0, 0 }));
            ASSERT_TRUE(tri.is_inside_cl({ 0, 1 }));
            ASSERT_TRUE(tri.is_inside_cl({ 1, 0 }));
            ASSERT_FALSE(tri.is_inside_cl({ 1.00001, 0 }));
            ASSERT_FALSE(tri.is_inside_cl({ -0.5, 0 }));
        }
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
