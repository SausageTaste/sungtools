#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "sung/basic/linalg.hpp"
#include "sung/basic/random.hpp"


namespace {

    sung::TVec3<double> vec_cast(const glm::dvec3& v) {
        return sung::TVec3<double>(v.x, v.y, v.z);
    }

    glm::dvec3 vec_cast(const sung::TVec3<double>& v) {
        return glm::dvec3(v.x(), v.y(), v.z());
    }

    sung::TVec4<double> vec_cast(const glm::dvec4& v) {
        return sung::TVec4<double>(v.x, v.y, v.z, v.w);
    }

    glm::dvec4 vec_cast(const sung::TVec4<double>& v) {
        return glm::dvec4(v.x(), v.y(), v.z(), v.w());
    }

    glm::dmat3 mat_cast(const sung::TMat3<double>& m) {
        return glm::dmat3(
            m.at(0, 0),
            m.at(1, 0),
            m.at(2, 0),
            m.at(0, 1),
            m.at(1, 1),
            m.at(2, 1),
            m.at(0, 2),
            m.at(1, 2),
            m.at(2, 2)
        );
    }

    glm::dmat4 mat_cast(const sung::TMat4<double>& m) {
        return glm::dmat4(
            m.at(0, 0),
            m.at(1, 0),
            m.at(2, 0),
            m.at(3, 0),
            m.at(0, 1),
            m.at(1, 1),
            m.at(2, 1),
            m.at(3, 1),
            m.at(0, 2),
            m.at(1, 2),
            m.at(2, 2),
            m.at(3, 2),
            m.at(0, 3),
            m.at(1, 3),
            m.at(2, 3),
            m.at(3, 3)
        );
    }


    class RandomTestGen {

    public:
        auto gen_scalar() { return rng_.gen(); }

        auto gen_vec3() {
            return sung::TVec3<double>{ rng_.gen(), rng_.gen(), rng_.gen() };
        }

        auto gen_vec4() {
            return sung::TVec4<double>{
                rng_.gen(), rng_.gen(), rng_.gen(), rng_.gen()
            };
        }

    private:
        sung::RandomRealNumGenerator<double> rng_{ -10000.f, 10000.f };
    };

}  // namespace


namespace {

    TEST(LinalgGlm, Vec3) {
        ::RandomTestGen rng;

        for (int i = 0; i < 1000; ++i) {
            const auto vs0 = rng.gen_vec3();
            const auto vg0 = vec_cast(vs0);
            ASSERT_DOUBLE_EQ(vg0.x, vs0.x());
            ASSERT_DOUBLE_EQ(vg0.y, vs0.y());
            ASSERT_DOUBLE_EQ(vg0.z, vs0.z());

            const auto vs1 = rng.gen_vec3();
            const auto vg1 = vec_cast(vs1);
            ASSERT_DOUBLE_EQ(vg1.x, vs1.x());
            ASSERT_DOUBLE_EQ(vg1.y, vs1.y());
            ASSERT_DOUBLE_EQ(vg1.z, vs1.z());
            ASSERT_DOUBLE_EQ(glm::dot(vg0, vg1), sung::dot(vs0, vs1));

            const auto vs_add = vs0 + vs1;
            const auto vg_add = vg0 + vg1;
            ASSERT_DOUBLE_EQ(vg_add.x, vs_add.x());
            ASSERT_DOUBLE_EQ(vg_add.y, vs_add.y());
            ASSERT_DOUBLE_EQ(vg_add.z, vs_add.z());

            const auto vs_cross = sung::cross(vs0, vs1);
            const auto vg_cross = glm::cross(vg0, vg1);
            ASSERT_DOUBLE_EQ(vg_cross.x, vs_cross.x());
            ASSERT_DOUBLE_EQ(vg_cross.y, vs_cross.y());
            ASSERT_DOUBLE_EQ(vg_cross.z, vs_cross.z());
        }

        return;
    }


    TEST(LinalgGlm, VectorTranslation) {
        using Vec3 = sung::TVec3<double>;
        using Mat4 = sung::TMat4<double>;
        ::RandomTestGen rng;

        for (int i = 0; i < 1000; ++i) {
            const auto v_sung = rng.gen_vec4();
            const auto v_glm = vec_cast(v_sung);

            const auto offset = rng.gen_vec3();
            const auto mat_glm = glm::translate(
                glm::dmat4(1), vec_cast(offset)
            );
            const auto mat_sung = Mat4::translate(offset);

            const auto col_glm = mat_glm[3];  // glm is column major
            const auto col_sung = mat_sung.column(3);

            ASSERT_DOUBLE_EQ(col_glm.x, col_sung.x());
            ASSERT_DOUBLE_EQ(col_glm.y, col_sung.y());
            ASSERT_DOUBLE_EQ(col_glm.z, col_sung.z());
            ASSERT_DOUBLE_EQ(col_glm.w, col_sung.w());

            const auto v2_glm = mat_glm * v_glm;
            const auto v2_sung = mat_sung * v_sung;

            ASSERT_DOUBLE_EQ(v2_glm.x, v2_sung.x());
            ASSERT_DOUBLE_EQ(v2_glm.y, v2_sung.y());
            ASSERT_DOUBLE_EQ(v2_glm.z, v2_sung.z());
            ASSERT_DOUBLE_EQ(v2_glm.w, v2_sung.w());

            const auto mat_inv_glm = glm::inverse(mat_glm);
            const auto mat_inv_sung = mat_sung.inverse().value();

            const auto col_inv_glm = mat_inv_glm[3];  // glm is column major
            const auto col_inv_sung = mat_inv_sung.column(3);

            ASSERT_DOUBLE_EQ(col_inv_glm.x, col_inv_sung.x());
            ASSERT_DOUBLE_EQ(col_inv_glm.y, col_inv_sung.y());
            ASSERT_DOUBLE_EQ(col_inv_glm.z, col_inv_sung.z());
            ASSERT_DOUBLE_EQ(col_inv_glm.w, col_inv_sung.w());

            const auto v3_glm = mat_inv_glm * v2_glm;
            const auto v3_sung = mat_inv_sung * v2_sung;

            ASSERT_NEAR(v_sung.x(), v3_sung.x(), 0.01);
            ASSERT_NEAR(v_sung.y(), v3_sung.y(), 0.01);
            ASSERT_NEAR(v_sung.z(), v3_sung.z(), 0.01);
            ASSERT_NEAR(v_sung.w(), v3_sung.w(), 0.01);

            ASSERT_DOUBLE_EQ(v3_glm.x, v3_sung.x());
            ASSERT_DOUBLE_EQ(v3_glm.y, v3_sung.y());
            ASSERT_DOUBLE_EQ(v3_glm.z, v3_sung.z());
            ASSERT_DOUBLE_EQ(v3_glm.w, v3_sung.w());
        }

        return;
    }


    TEST(LinalgGlm, VectorRotation) {
        using Vec3 = sung::TVec3<double>;
        using Mat3 = sung::TMat3<double>;
        using Mat4 = sung::TMat4<double>;
        constexpr double EPSILON = 1e-6;
        ::RandomTestGen rng;

        for (int i = 0; i < 1000; ++i) {
            const auto v_sung = rng.gen_vec3();
            const auto v_glm = vec_cast(v_sung);
            const auto axis_sung = rng.gen_vec3().normalize();
            const auto axis_glm = vec_cast(axis_sung);
            const auto angle = rng.gen_scalar();

            const auto rot_sung = Mat3::rotate_axis(axis_sung, angle);
            const auto rot_quat = glm::angleAxis(angle, axis_glm);
            const auto rot_glm = glm::mat3_cast(rot_quat);

            const auto v2_glm = rot_glm * v_glm;
            const auto v2_sung = rot_sung * v_sung;

            for (int r = 0; r < 3; ++r) {
                for (int c = 0; c < 3; ++c) {
                    ASSERT_NEAR(rot_glm[c][r], rot_sung.at(r, c), EPSILON)
                        << "row: " << r << ", col: " << c;
                }
            }

            ASSERT_NEAR(v2_glm.x, v2_sung.x(), EPSILON);
            ASSERT_NEAR(v2_glm.y, v2_sung.y(), EPSILON);
            ASSERT_NEAR(v2_glm.z, v2_sung.z(), EPSILON);

            const auto v3_glm = glm::inverse(rot_glm) * v2_glm;
            const auto v3_sung = rot_sung.inverse().value() * v2_sung;

            ASSERT_NEAR(v_sung.x(), v3_sung.x(), EPSILON);
            ASSERT_NEAR(v_sung.y(), v3_sung.y(), EPSILON);
            ASSERT_NEAR(v_sung.z(), v3_sung.z(), EPSILON);

            ASSERT_NEAR(v3_glm.x, v3_sung.x(), EPSILON);
            ASSERT_NEAR(v3_glm.y, v3_sung.y(), EPSILON);
            ASSERT_NEAR(v3_glm.z, v3_sung.z(), EPSILON);
        }

        return;
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
