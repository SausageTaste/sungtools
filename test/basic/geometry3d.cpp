#include "sung/basic/geometry3d.hpp"

#include <vector>

#include <gtest/gtest.h>

#include "sung/basic/random.hpp"


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
            const auto info = plane.find_seg_intersec(test_case.seg_);
            const auto inter = test_case.seg_.pos() +
                               test_case.seg_.dir().normalize() *
                                   info->distance_;

            EXPECT_DOUBLE_EQ(inter[0], test_case.ipoint_[0]);
            EXPECT_DOUBLE_EQ(inter[1], test_case.ipoint_[1]);
            EXPECT_DOUBLE_EQ(inter[2], test_case.ipoint_[2]);
        }
    }


    TEST(Geometry3D, Triangle3) {
        using Tri = sung::Triangle3;
        sung::RandomRealNumGenerator<double> rng{ -1000, 1000 };

        {
            const Tri tri{ { 0, 0, 0 }, { 1, 0, 0 }, { 0, 1, 0 } };
            ASSERT_DOUBLE_EQ(tri.area(), 0.5);
            ASSERT_DOUBLE_EQ(*tri.radius_circumcircle(), std::sqrt(2.0) * 0.5);

            const auto c = tri.circumcenter().value();
            ASSERT_NEAR(c.distance_sqr(tri.a()), c.distance_sqr(tri.b()), 1e-6);
            ASSERT_NEAR(c.distance_sqr(tri.a()), c.distance_sqr(tri.c()), 1e-6);

            const auto p = tri.plane();
            ASSERT_NEAR(p.calc_signed_dist(tri.a()), 0, 1e-6);
        }

        {
            const Tri tri{ { 0, 5, 3 }, { 1, 2, 4 }, { 3, 1, 2 } };
            ASSERT_NEAR(tri.area(), 4.743, 1e-3);
            ASSERT_NEAR(*tri.radius_circumcircle(), 2.674, 1e-3);

            const auto c = tri.circumcenter().value();
            ASSERT_NEAR(c.distance_sqr(tri.a()), c.distance_sqr(tri.b()), 1e-6);
            ASSERT_NEAR(c.distance_sqr(tri.a()), c.distance_sqr(tri.c()), 1e-6);

            const auto p = tri.plane();
            ASSERT_NEAR(p.calc_signed_dist(tri.a()), 0, 1e-6);
        }
    }


    TEST(Geometry3D, TriSoup) {
        sung::TriSoup3 soup;
        soup.add_vtx({ 0, 0, 0 });
        soup.add_vtx({ 2, 0, 0 });
        soup.add_vtx({ 0, 2, 0 });

        const sung::LineSegment3 ray{ { 1, 1, 1 }, { 0, 0, -2 } };
        const auto in = soup.find_seg_intersec(ray, false);
        ASSERT_TRUE(in.has_value());

        auto p = ray.pos() + ray.dir().normalize() * in->distance_;
        ASSERT_DOUBLE_EQ(p[0], 1);
        ASSERT_DOUBLE_EQ(p[1], 1);
        ASSERT_DOUBLE_EQ(p[2], 0);
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
