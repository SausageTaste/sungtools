#include <chrono>
#include <random>

#include <gtest/gtest.h>

#include "sung/general/angle.hpp"


namespace {

    template <typename T>
    class RandomDoubleGenerator {

    public:
        RandomDoubleGenerator(T lower_bound, T upper_bound)
            : unif_(lower_bound, upper_bound) {
            const auto now = std::chrono::high_resolution_clock::now();
            const auto time_seed = now.time_since_epoch().count();
            std::seed_seq ss{ uint32_t(time_seed & 0xffffffff),
                              uint32_t(time_seed >> 32) };
            rng_.seed(ss);
        }

        T gen() { return unif_(rng_); }

    private:
        std::mt19937_64 rng_;
        std::uniform_real_distribution<T> unif_;
    };

}  // namespace


// Diff calculation test
namespace {

    TEST(Angle, ShortestDiffCalcEq) {
        ::RandomDoubleGenerator<double> rng{ -1e3, 1e3 };

        for (int i = 0; i < 100; ++i) {
            const auto a_deg = rng.gen();
            const auto b_deg = rng.gen();
            const auto a_rad = sung::to_radians(a_deg);
            const auto b_rad = sung::to_radians(b_deg);

            EXPECT_DOUBLE_EQ(
                sung::calc_rad_shortest_diff_mod(a_rad, b_rad),
                sung::calc_rad_shortest_diff_floor(a_rad, b_rad)
            );
        }
    }


    TEST(Angle, BinaryOperation) {
        using Angle = sung::TAngle<double>;
        ::RandomDoubleGenerator<double> rng{ -1e3, 1e3 };

        for (int i = 0; i < 100; ++i) {
            const auto a_deg = rng.gen();
            const auto b_deg = rng.gen();

            const auto a = Angle::from_deg(a_deg);
            const auto b = Angle::from_deg(b_deg);

            EXPECT_NEAR(
                (a + b).rad(), Angle::from_deg(a_deg + b_deg).rad(), 1e-10
            );
            EXPECT_NEAR(
                (a - b).rad(), Angle::from_deg(a_deg - b_deg).rad(), 1e-10
            );
            EXPECT_NEAR(
                (a * b_deg).rad(), Angle::from_deg(a_deg).rad() * b_deg, 1e-10
            );
            EXPECT_NEAR(
                (a / b_deg).rad(), Angle::from_deg(a_deg).rad() / b_deg, 1e-10
            );

            const auto a2b = a.calc_short_diff_to(b);
            EXPECT_NEAR(b.calc_short_diff_to(a + a2b).rad(), 0, 1e-10);
        }
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
