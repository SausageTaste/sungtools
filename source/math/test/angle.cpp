#include "sung/math/angle.hpp"

#include <chrono>
#include <iostream>
#include <random>


using sung::Angle;


// Randomized test
namespace {

    constexpr double EPSILON = 1e-8;


    enum TestResult {
        TEST_RESULT_SUCCESS = 0,
        TEST_RESULT_TEST_DIFF_FAIL,
        TEST_RESULT_TEST_ADD_FAIL,
    };

}


// Randomized test
namespace {

    class RandomDoubleGenerator {

    public:
        RandomDoubleGenerator(double lower_bound, double upper_bound)
            : unif_(lower_bound, upper_bound)
        {
            const uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            const std::seed_seq ss { uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
            rng_.seed(ss);
        }

        double gen() {
            return unif_(rng_);
        }

    private:
        std::mt19937_64 rng_;
        std::uniform_real_distribution<double> unif_;

    };


    bool test_diff(const sung::Angle& a, const sung::Angle& b) {
        const auto diff = a.calc_short_diff(b);
        const auto a_plus_diff = a + diff;

        if (!b.is_equivalent(a_plus_diff, ::EPSILON)) {
            const auto residual = b.normalize_neg().rad() - a_plus_diff.normalize_neg().rad();
            std::cout << "Test `test_diff` failed"
                << "\n    a: " << a.deg()
                << "\n    b: " << b.deg()
                << "\n    diff: " << diff.deg()
                << "\n    a + diff: " << a_plus_diff.deg()
                << "\n    residual: " << residual
                << std::endl;
        }

        return true;
    }


    TestResult randomized_test() {
        ::RandomDoubleGenerator rng{ -1e6, 1e6 };

        constexpr auto TEST_DURATION = std::chrono::seconds(1);
        const auto start_time = std::chrono::high_resolution_clock::now();
        size_t count = 0;

        for (;;) {
            const auto a = Angle::from_rad(rng.gen());
            const auto b = Angle::from_rad(rng.gen());

            if (!test_diff(a, b))
                return TEST_RESULT_TEST_DIFF_FAIL;

            ++count;
            if (std::chrono::high_resolution_clock::now() - start_time > TEST_DURATION)
                break;
        }

        const auto seconds = std::chrono::duration_cast<std::chrono::milliseconds>(TEST_DURATION).count() / 1000.0;
        std::cout << "Randomized test passed " << std::fixed << count / seconds << " times per seconds" << std::endl;
        return TEST_RESULT_SUCCESS;
    }

}


// Addition test
namespace {

    // All in degrees
    struct AdditionTestCase {
        double a_;
        double b_;
        double sum_;
    };


    TestResult addition_test() {
        const std::vector<AdditionTestCase> test_cases{
            {20, 60, 80},
            {370, 230, 240},
            {370, 230, -120},
            {15.47, -84.354, -68.884},
        };

        for (auto& test_case : test_cases) {
            const auto a = Angle::from_deg(test_case.a_);
            const auto b = Angle::from_deg(test_case.b_);
            const auto sum = a + b;

            if (!sum.is_equivalent(Angle::from_deg(test_case.sum_), ::EPSILON)) {
                std::cout << "Test `addition_test` failed"
                    << "\n    a: " << a.deg()
                    << "\n    b: " << b.deg()
                    << "\n    sum: " << sum.deg()
                    << "\n    expected sum: " << test_case.sum_
                    << std::endl;
                return TEST_RESULT_TEST_ADD_FAIL;
            }
        }

        return TEST_RESULT_SUCCESS;
    }

}


int main() {
    TestResult result;

    result = ::addition_test();
    if (TEST_RESULT_SUCCESS != result)
        return result;

    result = ::randomized_test();
    if (TEST_RESULT_SUCCESS != result)
        return result;

    return 0;
}
