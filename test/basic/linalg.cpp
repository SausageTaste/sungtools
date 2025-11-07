#include <gtest/gtest.h>

#include "sung/basic/angle.hpp"
#include "sung/basic/linalg.hpp"
#include "sung/basic/random.hpp"

// Static ASSERT SIMILAR Double
#define SASSERT_SIMILARD(a, b) \
    static_assert(sung::are_similiar((T)(a), (T)(b), (T)(1e-10)), "")

#define ASSERT_MAT_NEAR(lhs, rhs, rows, cols, epsilon)                \
    do {                                                              \
        for (int row = 0; row < (rows); ++row) {                      \
            for (int col = 0; col < (cols); ++col) {                  \
                ASSERT_NEAR(                                          \
                    (lhs).at(row, col), (rhs).at(row, col), (epsilon) \
                );                                                    \
            }                                                         \
        }                                                             \
    } while (false)


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


    TEST(Linalg, Matrix3x3Operators) {
        using T = double;
        using Mat3 = sung::TMat3<T>;

        {
            constexpr Mat3 m{ { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };

            constexpr auto det = m.determinant();
            SASSERT_SIMILARD(det, 0);

            constexpr auto inv = m.inverse();
            static_assert(!inv.has_value(), "");
        }

        {
            constexpr Mat3 m{ { 456, 231, 8 }, { 635, 2, 4 }, { 632, 48, 9 } };
            constexpr auto det = m.determinant();
            SASSERT_SIMILARD(det, -581813.0);

            constexpr auto inv = m.inverse();
            SASSERT_SIMILARD(inv->at(0, 0), 0.00029906516354911285965);
            SASSERT_SIMILARD(inv->at(0, 1), 0.002913307196642220095);
            SASSERT_SIMILARD(inv->at(0, 2), -0.0015606388994401981396);
            SASSERT_SIMILARD(inv->at(1, 0), 0.0054777050358104751868);
            SASSERT_SIMILARD(inv->at(1, 1), 0.0016362645729813531142);
            SASSERT_SIMILARD(inv->at(1, 2), -0.0055962998420454682165);
            SASSERT_SIMILARD(inv->at(2, 0), -0.050215447231326904026);
            SASSERT_SIMILARD(inv->at(2, 1), -0.2133056497534431166);
            SASSERT_SIMILARD(inv->at(2, 2), 0.25054957520715418872);

            static_assert(Mat3::identity().are_similar(m * (*inv), 1e-10), "");
            static_assert(m.are_similar(*inv->inverse(), 1e-10), "");

            constexpr Mat3::Vec3 v{ 1, 2, 3 };
            static_assert(v.are_similar(inv.value() * m * v, 1e-10), "");
        }

        {
            constexpr Mat3 m{ { -0.123, 0.456, 1.2 },
                              { 1.635, -0.0002, 0 },
                              { 0.748, -2.2, 155 } };

            constexpr auto det = m.determinant();
            SASSERT_SIMILARD(det, -119.87420748);

            constexpr auto inv = m.inverse();
            SASSERT_SIMILARD(inv->at(0, 0), 0.00025860442084817989919);
            SASSERT_SIMILARD(inv->at(0, 1), 0.61164116569640573693);
            SASSERT_SIMILARD(inv->at(0, 2), -0.0000020020987420504277357);
            SASSERT_SIMILARD(inv->at(1, 0), 2.1140911404338737571);
            SASSERT_SIMILARD(inv->at(1, 1), 0.16652956811689947032);
            SASSERT_SIMILARD(inv->at(1, 2), -0.016367157216262248444);
            SASSERT_SIMILARD(inv->at(2, 0), 0.030005206921598244049);
            SASSERT_SIMILARD(inv->at(2, 1), -0.00058801640054021068715);
            SASSERT_SIMILARD(inv->at(2, 2), 0.0062193145270585942399);

            static_assert(Mat3::identity().are_similar(m * (*inv), 1e-10), "");
            static_assert(m.are_similar(*inv->inverse(), 1e-10), "");

            constexpr Mat3::Vec3 v{ 1, 2, 3 };
            static_assert(v.are_similar(inv.value() * m * v, 1e-10), "");
        }
    }


    TEST(Linalg, Matrix3x3Rand) {
        using T = double;
        using Mat3 = sung::TMat3<T>;

        sung::RandomRealNumGenerator<T> rg{ -10, 10 };
        const auto IDENTITY = Mat3::identity();

        for (int i = 0; i < 100000; ++i) {
            const Mat3 m{
                { rg(), rg(), rg() },
                { rg(), rg(), rg() },
                { rg(), rg(), rg() },
            };

            if (const auto m_inv = m.inverse()) {
                const auto m_invinv = m_inv->inverse();
                ASSERT_TRUE(m_invinv.has_value());
                // ASSERT_MAT_NEAR(IDENTITY, (m * m_inv.value()), 3, 3, 1e-6);
                // ASSERT_MAT_NEAR(m, m_invinv.value(), 3, 3, 1e-1);
            }
        }
    }


    TEST(Linalg, Matrix4x4Operators) {
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


    TEST(Linalg, Matrix4x4Transform) {
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
                             Mat::rotate_axis({ 1, 0, 0 }, -angle) * Vec4(v, 0);
        EXPECT_NEAR(result2.x(), 0, EPSILON);
        EXPECT_NEAR(result2.y(), 0, EPSILON);
        EXPECT_NEAR(result2.z(), -1, EPSILON);
        EXPECT_NEAR(result2.w(), 0, EPSILON);

        constexpr auto result3 = Mat::translate(1, 2, 3) *
                                 Mat::translate(1, -2, 3) * Vec4(v, 1);
        static_assert(result3.are_similar({ 3, 0, 6, 1 }, 0.001f), "");
    }


    TEST(Linalg, Matrix4x4Rand) {
        using T = double;
        using Mat4 = sung::TMat4<T>;

        sung::RandomRealNumGenerator<T> rg{ -10, 10 };
        const auto IDENTITY = Mat4::identity();

        for (int i = 0; i < 100000; ++i) {
            const Mat4 m{
                { rg(), rg(), rg(), rg() },
                { rg(), rg(), rg(), rg() },
                { rg(), rg(), rg(), rg() },
                { rg(), rg(), rg(), rg() },
            };

            if (const auto m_inv = m.inverse()) {
                const auto m_invinv = m_inv->inverse();
                ASSERT_TRUE(m_invinv.has_value());
                // ASSERT_MAT_NEAR(IDENTITY, m * m_inv.value(), 4, 4, 1e-6);
                // ASSERT_MAT_NEAR(m, m_invinv.value(), 4, 4, 1);
            }
        }
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
