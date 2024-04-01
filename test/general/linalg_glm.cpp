#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "sung/general/linalg.hpp"
#include "sung/general/random.hpp"


namespace {

    sung::TVec3<float> vec_cast(const glm::vec3& v) {
        return sung::TVec3<float>(v.x, v.y, v.z);
    }

    glm::vec3 vec_cast(const sung::TVec3<float>& v) {
        return glm::vec3(v.x(), v.y(), v.z());
    }

    sung::TVec4<float> vec_cast(const glm::vec4& v) {
        return sung::TVec4<float>(v.x, v.y, v.z, v.w);
    }

    glm::vec4 vec_cast(const sung::TVec4<float>& v) {
        return glm::vec4(v.x(), v.y(), v.z(), v.w());
    }

}  // namespace


namespace {

    TEST(LinalgGlm, Vec3) {
        sung::RandomRealNumGenerator<float> rng(-10000.f, 10000.f);

        for (int i = 0; i < 1000; ++i) {
            const glm::vec3 vg0(rng.gen(), rng.gen(), rng.gen());
            const auto vs0 = vec_cast(vg0);

            EXPECT_FLOAT_EQ(vg0.x, vs0.x());
            EXPECT_FLOAT_EQ(vg0.y, vs0.y());
            EXPECT_FLOAT_EQ(vg0.z, vs0.z());

            const sung::TVec3<float> vs1(rng.gen(), rng.gen(), rng.gen());
            const auto vg1 = vec_cast(vs1);

            EXPECT_FLOAT_EQ(vg1.x, vs1.x());
            EXPECT_FLOAT_EQ(vg1.y, vs1.y());
            EXPECT_FLOAT_EQ(vg1.z, vs1.z());

            EXPECT_FLOAT_EQ(glm::dot(vg0, vg1), sung::dot(vs0, vs1));
        }
    }


    TEST(LinalgGlm, VectorTranslation) {
        using Vec3 = sung::TVec3<float>;
        using Mat4 = sung::TMat4<float>;
        sung::RandomRealNumGenerator<float> rng(-10000.f, 10000.f);

        for (int i = 0; i < 1000; ++i) {
            const glm::vec4 v_glm(rng.gen(), rng.gen(), rng.gen(), 1);
            const auto v_sung = vec_cast(v_glm);

            const glm::vec3 offset(rng.gen(), rng.gen(), rng.gen());
            const auto mat_glm = glm::translate(glm::mat4(1.f), offset);
            const auto mat_sung = Mat4::translate(offset.x, offset.y, offset.z);

            const auto col_glm = mat_glm[3];  // glm is column major
            const auto col_sung = mat_sung.column(3);

            EXPECT_FLOAT_EQ(col_glm.x, col_sung.x());
            EXPECT_FLOAT_EQ(col_glm.y, col_sung.y());
            EXPECT_FLOAT_EQ(col_glm.z, col_sung.z());
            EXPECT_FLOAT_EQ(col_glm.w, col_sung.w());

            const auto v2_glm = mat_glm * v_glm;
            const auto v2_sung = mat_sung * v_sung;

            EXPECT_FLOAT_EQ(v2_glm.x, v2_sung.x());
            EXPECT_FLOAT_EQ(v2_glm.y, v2_sung.y());
            EXPECT_FLOAT_EQ(v2_glm.z, v2_sung.z());
            EXPECT_FLOAT_EQ(v2_glm.w, v2_sung.w());
        }
    }


    TEST(LinalgGlm, VectorRotation) {
        using Vec3 = sung::TVec3<float>;
        using Mat4 = sung::TMat4<float>;
        constexpr float EPSILON = 1e-2f;
        sung::RandomRealNumGenerator<float> rng(-10000.f, 10000.f);

        for (int i = 0; i < 1000; ++i) {
            const glm::vec4 v_glm(rng.gen(), rng.gen(), rng.gen(), 0);
            const auto v_sung = vec_cast(v_glm);

            const glm::vec3 axis_glm(rng.gen(), rng.gen(), rng.gen());
            const auto axis_glm_n = glm::normalize(axis_glm);
            const auto axis_sung_n = vec_cast(axis_glm_n);

            const auto angle = rng.gen();

            const auto rot_glm = glm::mat4_cast(
                glm::angleAxis(angle, axis_glm_n)
            );
            const auto rot_sung = Mat4::rotate_axis(axis_sung_n, angle);

            const auto v2_glm = rot_glm * v_glm;
            const auto v2_sung = rot_sung * v_sung;

            for (int r = 0; r < 4; ++r) {
                for (int c = 0; c < 4; ++c) {
                    EXPECT_NEAR(rot_glm[c][r], rot_sung.at(r, c), EPSILON)
                        << "row: " << r << ", col: " << c;
                }
            }

            EXPECT_NEAR(v2_glm.x, v2_sung.x(), EPSILON);
            EXPECT_NEAR(v2_glm.y, v2_sung.y(), EPSILON);
            EXPECT_NEAR(v2_glm.z, v2_sung.z(), EPSILON);
            EXPECT_NEAR(v2_glm.w, v2_sung.w(), EPSILON);
        }
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
