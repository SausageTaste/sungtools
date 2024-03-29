#include <gtest/gtest.h>

#include "sung/general/angle.hpp"
#include "sung/general/linalg.hpp"


namespace {

    TEST(Linalg, VectorOperators) {
        constexpr sung::TVec3<float> a{ 1, 2, 3 };
        constexpr sung::TVec3<float> b{ 4, 5, 6 };
        constexpr auto c = a + b;

        constexpr sung::TVec4<float> d{ 1, 2, 3, 4 };
        constexpr auto d3 = sung::TVec3<double>{ d };
        constexpr auto d4 = sung::TVec4<float>{ d3, 1 };
        constexpr auto d5 = sung::TVec4<double>{ d4 };

        constexpr auto m = sung::TMat4<float>::translate(2, 3, 4);
        constexpr auto m00 = m.at(0, 3);
        constexpr auto translated = m * d4;
    }


    TEST(Linalg, MatrixOperators) {
        using T = float;
        using Mat4 = sung::TMat4<T>;

        Mat4 m0{ { 1, 2, 3, 4 },
                 { 5, 6, 7, 8 },
                 { 9, 10, 11, 12 },
                 { 13, 14, 15, 16 } };

        m0.set_column(3, 101, 102, 103, 104);
        EXPECT_FLOAT_EQ(m0.at(0, 3), 101);
        EXPECT_FLOAT_EQ(m0.at(1, 3), 102);
        EXPECT_FLOAT_EQ(m0.at(2, 3), 103);
        EXPECT_FLOAT_EQ(m0.at(3, 3), 104);

        m0.set_row(3, 201, 202, 203, 204);
        EXPECT_FLOAT_EQ(m0.at(3, 0), 201);
        EXPECT_FLOAT_EQ(m0.at(3, 1), 202);
        EXPECT_FLOAT_EQ(m0.at(3, 2), 203);
        EXPECT_FLOAT_EQ(m0.at(3, 3), 204);
    }


    TEST(Linalg, MatrixTransform) {
        using Vec3 = sung::TVec4<float>;
        using Vec4 = sung::TVec4<float>;
        using Mat = sung::TMat4<float>;

        constexpr double EPSILON = 1e-6;

        constexpr sung::TVec3<float> v{ 1, 0, 0 };
        constexpr auto angle = sung::to_radians(90.f);

        const auto result0 = Mat::rotate_axis({ 0, 0, 1 }, angle) * Vec4(v, 0);
        EXPECT_NEAR(result0.x(), 0, EPSILON);
        EXPECT_NEAR(result0.y(), 1, EPSILON);
        EXPECT_NEAR(result0.z(), 0, EPSILON);
        EXPECT_NEAR(result0.w(), 0, EPSILON);

        const auto result1 = Mat::rotate_axis({ 0, 1, 0 }, angle) * Vec4(v, 0);
        EXPECT_NEAR(result1.x(), 0, EPSILON);
        EXPECT_NEAR(result1.y(), 0, EPSILON);
        EXPECT_NEAR(result1.z(), -1, EPSILON);
        EXPECT_NEAR(result1.w(), 0, EPSILON);

        const auto result2 = Mat::rotate_axis({ 0, 1, 0 }, angle) *
                             Mat::rotate_axis({ 1, 0, 0 }, -angle) * Vec4(v, 1);
        EXPECT_NEAR(result2.x(), 0, EPSILON);
        EXPECT_NEAR(result2.y(), 0, EPSILON);
        EXPECT_NEAR(result2.z(), -1, EPSILON);
        EXPECT_NEAR(result2.w(), 0, EPSILON);

        constexpr auto result3 = Mat::translate(1, 2, 3) *
                                 Mat::translate(1, -2, 3) * Vec4(v, 1);
        static_assert(result3.are_similar({ 3, 0, 6, 1 }, 0.001f), "");
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
