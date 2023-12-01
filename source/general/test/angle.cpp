#include "sung/general/angle.hpp"

#include <chrono>
#include <iostream>
#include <random>


namespace {

    enum TestResult {
        TEST_RESULT_SUCCESS = 0,
        TEST_RESULT_TEST_DIFF_FLOAT_FAIL,
        TEST_RESULT_TEST_DIFF_DOUBLE_FAIL,
        TEST_RESULT_TEST_ADD_FAIL,
    };


    template <typename T>
    class RandomDoubleGenerator {

    public:
        RandomDoubleGenerator(T lower_bound, T upper_bound)
            : unif_(lower_bound, upper_bound)
        {
            const auto time_seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            const std::seed_seq ss{ uint32_t(time_seed & 0xffffffff), uint32_t(time_seed >> 32) };
            rng_.seed(ss);
        }

        T gen() {
            return unif_(rng_);
        }

    private:
        std::mt19937_64 rng_;
        std::uniform_real_distribution<T> unif_;

    };

}


// Diff calculation test
namespace {

    template <typename T>
    void print_diff_test_variables(const sung::TAngle<T> a, const sung::TAngle<T> b) {
        const auto d = a.calc_short_diff(b);
        const auto a_plus_d = a + d;
        const auto b_ad_diff = b.calc_short_diff(a_plus_d);

        std::cout << "Test `randomized_test` failed (T = " << typeid(T).name() << ")"
            << "\n    a:              " << a.rad() << " (" << a.normalize_neg().rad() << ")"
            << "\n    b:              " << b.rad() << " (" << b.normalize_neg().rad() << ")"
            << "\n    d:              " << d.rad() << " (" << d.normalize_neg().rad() << ")"
            << "\n    a+d:            " << a_plus_d.rad() << " (" << a_plus_d.normalize_neg().rad()     << ")"
            << "\n    Diff of a+d, b: " << b_ad_diff.rad() << " (" << b_ad_diff.normalize_neg().rad() << ")"
            << std::endl;
    }


    TestResult diff_calc_test() {
        ::RandomDoubleGenerator<double> rng{ -1e3, 1e3 };

        constexpr auto TEST_DURATION = std::chrono::seconds(1);
        const auto start_time = std::chrono::high_resolution_clock::now();
        size_t count = 0;

        for (;;) {
            const auto a_deg = rng.gen();
            const auto b_deg = rng.gen();

            const auto ad = sung::TAngle<double>::from_rad(a_deg);
            const auto bd = sung::TAngle<double>::from_rad(b_deg);
            const auto dd = ad.calc_short_diff(bd);
            const auto a_plus_d_d = ad + dd;
            if (!bd.is_equivalent(a_plus_d_d, 1e-10)) {
                ::print_diff_test_variables(ad, bd);
                return TEST_RESULT_TEST_DIFF_DOUBLE_FAIL;
            }

            const auto af = sung::TAngle<float>::from_rad(static_cast<float>(a_deg));
            const auto bf = sung::TAngle<float>::from_rad(static_cast<float>(b_deg));
            const auto df = af.calc_short_diff(bf);
            const auto a_plus_d_f = af + df;
            if (!bf.is_equivalent(a_plus_d_f, 1e-3f)) {
                ::print_diff_test_variables(af, bf);
                return TEST_RESULT_TEST_DIFF_FLOAT_FAIL;
            }

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
    template <typename T>
    struct AdditionTestCase {
        T a_;
        T b_;
        T sum_;
    };


    template <typename T>
    TestResult addition_test(T epsilon) {
        const std::vector<AdditionTestCase<T>> test_cases{
            {20, 60, 80},
            {370, 230, 240},
            {370, 230, -120},
            {(T)15.47, (T)-84.354, (T)-68.884},
        };

        for (auto& test_case : test_cases) {
            const auto a = sung::TAngle<T>::from_deg(test_case.a_);
            const auto b = sung::TAngle<T>::from_deg(test_case.b_);
            const auto sum = a + b;

            if (!sum.is_equivalent(sung::TAngle<T>::from_deg(test_case.sum_), epsilon)) {
                std::cout << "Test `addition_test` failed (T = " << typeid(T).name() << ")"
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

    result = ::addition_test<float>(1e-5f);
    if (TEST_RESULT_SUCCESS != result)
        return result;

    result = ::addition_test<double>(1e-6);
    if (TEST_RESULT_SUCCESS != result)
        return result;

    result = ::diff_calc_test();
    if (TEST_RESULT_SUCCESS != result)
        return result;

    return 0;
}
