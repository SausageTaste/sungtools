#include <gtest/gtest.h>

#include "sung/basic/aabb.hpp"


namespace {

    TEST(AABB, FloatAABB1) {
        using T = float;
        using AABB1 = sung::Aabb1D<T>;

        constexpr AABB1 aabb{ -1, 2 };

        static_assert(aabb.mini() == -1, "AABB1::mini failed");
        static_assert(aabb.maxi() == 2, "AABB1::maxi failed");
        static_assert(aabb.len() == 3, "AABB1::len failed");
        static_assert(aabb.mid() == 0.5, "AABB1::mid failed");

        static_assert(!aabb.is_inside_op(2), "AABB1::is_inside_op failed");
        static_assert(aabb.is_inside_cl(2), "AABB1::is_inside_cl failed");

        constexpr auto aabb2 = AABB1((T)-2.3, (T)1.4);
        constexpr auto intersection = aabb.make_intersection(aabb2);
        static_assert(
            intersection->are_similar(AABB1(-1, (T)1.4), (T)0.001),
            "AABB1::make_intersection failed"
        );

        constexpr auto aabb3 = AABB1((T)5.3, (T)2.4);
        constexpr auto intersection2 = aabb.make_intersection(aabb3);
        static_assert(
            intersection2 == sung::nullopt, "AABB1::make_intersection failed"
        );
    }


    TEST(AABB, FloatAABB2) {
        using T = float;
        using AABB2 = sung::Aabb2D<T>;

        constexpr AABB2 a{ 2, -1, 3, 5 };
        constexpr AABB2 b{ (T)-2.3, (T)1.4, 7, (T)3.14 };

        static_assert(a.x_min() == -1, "AABB2::x_min failed");
        static_assert(a.y_min() == 3, "AABB2::y_min failed");
        static_assert(a.x_max() == 2, "AABB2::x_max failed");
        static_assert(a.y_max() == 5, "AABB2::y_max failed");

        static_assert(a.width() == 3, "AABB2::width failed");
        static_assert(a.height() == 2, "AABB2::maxx failed");
        static_assert(a.area() == 6, "AABB2::miny failed");

        static_assert(a.is_inside_cl(2, 5), "AABB2::is_inside_cl failed");
        static_assert(!a.is_inside_op(2, 5), "AABB2::is_inside_cl failed");
        static_assert(!a.is_inside_cl(2, 6), "AABB2::is_inside_cl failed");
        static_assert(!a.is_inside_op(2, 6), "AABB2::is_inside_cl failed");

        constexpr auto inter1 = a.make_intersection(b);
        static_assert(
            inter1->are_similar(AABB2{ -1, (T)1.4, 5, (T)3.14 }, (T)0.001),
            "AABB2::make_intersection failed"
        );

        constexpr auto a_expanded = a.get_expanded_to_span(12, -12);
        static_assert(
            a_expanded.are_similar(AABB2{ 12, -1, -12, 5 }, (T)0.001),
            "AABB2::get_expanded_to_span failed"
        );
    }


    TEST(AABB, FloatAABB3) {
        using T = float;
        using Vec3 = sung::TVec3<T>;
        using AABB3 = sung::Aabb3D<T>;

        constexpr AABB3 a{ 2, -1, 3, 5, 7, 11 };
        constexpr AABB3 b{ Vec3{ -2, 7, 5 }, Vec3{ 1, 3, 13 } };

        static_assert(a.x_min() == -1, "AABB3::x_min failed");
        static_assert(a.y_min() == 3, "AABB3::y_min failed");
        static_assert(a.z_min() == 7, "AABB3::z_min failed");
        static_assert(a.x_max() == 2, "AABB3::x_max failed");
        static_assert(a.y_max() == 5, "AABB3::y_max failed");
        static_assert(a.z_max() == 11, "AABB3::z_max failed");

        static_assert(a.x_len() == 3, "AABB3::x_len failed");
        static_assert(a.y_len() == 2, "AABB3::y_len failed");
        static_assert(a.z_len() == 4, "AABB3::z_len failed");
        static_assert(a.volume() == 24, "AABB3::volume failed");

        static_assert(
            a.is_inside_cl(Vec3{ 2, 5, 11 }), "AABB3::is_inside_cl failed"
        );
        static_assert(
            !a.is_inside_op(Vec3{ 2, 5, 11 }), "AABB3::is_inside_op failed"
        );
        static_assert(
            !a.is_inside_cl(Vec3{ 2, 5, 12 }), "AABB3::is_inside_cl failed"
        );
        static_assert(
            !a.is_inside_op(Vec3{ 2, 5, 12 }), "AABB3::is_inside_op failed"
        );

        constexpr auto inter1 = a.make_intersection(b);
        static_assert(
            inter1->are_similar(AABB3{ -1, 1, 3, 5, 7, 11 }, 0),
            "AABB3::make_intersection failed"
        );

        constexpr auto a_expanded = a.get_expanded_to_span(Vec3{ 12, -12, 0 });
        static_assert(
            a_expanded.are_similar(AABB3{ 12, -1, -12, 5, 0, 11 }, (T)0.001),
            "AABB3::get_expanded_to_span failed"
        );
    }


    TEST(AABB, Aabb2DLazyInit) {
        using AABB = sung::Aabb2DLazyInit<double>;
        using Vec2 = AABB::Vec2;

        AABB aabb;

        aabb.set_or_expand(1, 2);
        aabb.set_or_expand(9, 9);
        ASSERT_EQ(aabb.x_min(), 1);
        ASSERT_EQ(aabb.y_min(), 2);
        ASSERT_EQ(aabb.x_max(), 9);
        ASSERT_EQ(aabb.y_max(), 9);

        aabb.reset();
        aabb.set_or_expand(2, 3);
        ASSERT_EQ(aabb.x_min(), 2);
        ASSERT_EQ(aabb.y_min(), 3);
        ASSERT_EQ(aabb.x_max(), 2);
        ASSERT_EQ(aabb.y_max(), 3);

        aabb.offset(1, 2);
        ASSERT_EQ(aabb.x_min(), 3);
        ASSERT_EQ(aabb.y_min(), 5);

        aabb.offset(-Vec2(1, 1));
        ASSERT_EQ(aabb.x_min(), 2);
        ASSERT_EQ(aabb.y_min(), 4);
    }


    TEST(AABB, Aabb3DLazyInit) {
        sung::Aabb3DLazyInit<double> aabb;

        aabb.set_or_expand(1, 2, 3);
        aabb.set_or_expand(9, 9, 9);
        ASSERT_EQ(aabb.x_min(), 1);
        ASSERT_EQ(aabb.y_min(), 2);
        ASSERT_EQ(aabb.z_min(), 3);
        ASSERT_EQ(aabb.x_max(), 9);
        ASSERT_EQ(aabb.y_max(), 9);
        ASSERT_EQ(aabb.z_max(), 9);

        aabb.reset();
        aabb.set_or_expand(2, 3, 4);
        ASSERT_EQ(aabb.x_min(), 2);
        ASSERT_EQ(aabb.y_min(), 3);
        ASSERT_EQ(aabb.z_min(), 4);
        ASSERT_EQ(aabb.x_max(), 2);
        ASSERT_EQ(aabb.y_max(), 3);
        ASSERT_EQ(aabb.z_max(), 4);

        aabb.offset(1, 2, 3);
        ASSERT_EQ(aabb.x_min(), 3);
        ASSERT_EQ(aabb.y_min(), 5);
        ASSERT_EQ(aabb.z_min(), 7);

        aabb.offset(-sung::TVec3<double>(1, 1, 1));
        ASSERT_EQ(aabb.x_min(), 2);
        ASSERT_EQ(aabb.y_min(), 4);
        ASSERT_EQ(aabb.z_min(), 6);
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
