#include "sung/general/geometry3d.hpp"

#include <vector>

#include <gtest/gtest.h>


namespace {

    TEST(Geometry3D, LineSegment3) {
        sung::LineSegment3 line({ 1, 2, 3 }, { 4, 5, 6 });
        EXPECT_DOUBLE_EQ(line.len_sqr(), 77);
        EXPECT_DOUBLE_EQ(line.len(), std::sqrt(77));
    }


    TEST(Geometry3D, Plane3) {
        sung::Plane3 plane({ 0, 0, 0 }, { 0, 1, 0 });
        EXPECT_DOUBLE_EQ(plane.calc_signed_dist({ 1, 0, 3 }), 0);
        EXPECT_DOUBLE_EQ(plane.calc_signed_dist({ 5, 0, 2 }), 0);
    }


    struct PlaneSegInterTestCase {
        sung::LineSegment3 seg_;
        sung::TVec3<double> ipoint_;  // Intersection point
    };

    TEST(Geometry3D, PlaneSegIntersection3) {
        using Seg = sung::LineSegment3;
        using Vec3 = Seg::Vec3;

        const sung::Plane3 plane({ 0, 0, 0 }, { 0, 1, 0 });
        const std::vector<PlaneSegInterTestCase> test_cases{
            { Seg{ { 4, 2, 6 }, { 0, -5, 0 } }, Vec3{ 4, 0, 6 } },
            { Seg{ { 1, 3, 1 }, { 0, -3, 0 } }, Vec3{ 1, 0, 1 } },
        };

        for (auto& test_case : test_cases) {
            const auto info = plane.find_intersection(test_case.seg_);
            const auto inter = test_case.seg_.pos() +
                               test_case.seg_.dir().normalize() *
                                   info->distance_;

            EXPECT_DOUBLE_EQ(inter[0], test_case.ipoint_[0]);
            EXPECT_DOUBLE_EQ(inter[1], test_case.ipoint_[1]);
            EXPECT_DOUBLE_EQ(inter[2], test_case.ipoint_[2]);
        }
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
