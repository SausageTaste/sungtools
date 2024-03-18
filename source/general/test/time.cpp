#include "sung/general/time.hpp"

#include <functional>
#include <iostream>
#include <thread>


namespace {

    enum TestResult {
        TEST_RESULT_SUCCESS = 0,
        TEST_RESULT_FAIL_INVALID_MIN,
        TEST_RESULT_FAIL_SLEEP,
    };


    TestResult test_timepoint() {
        const auto tp = sung::TimePoint::from_now();
        std::cout << "Time point of now      : " << tp.to_datetime_text()
                  << " (" << std::fixed << tp.to_total_seconds() << ")\n";

        const auto via_total_sec = sung::TimePoint::from_total_sec(
            tp.to_total_seconds()
        );
        std::cout << "Time point of total sec: " << tp.to_datetime_text()
                  << " (" << via_total_sec.to_total_seconds() << ")\n";

        const auto via_time_t = sung::TimePoint::from_time_t(tp.to_time_t());
        std::cout << "Time point of time_t   : " << tp.to_datetime_text()
                  << " (" << via_time_t.to_total_seconds() << ")\n";

        return TEST_RESULT_SUCCESS;
    }

    TestResult test_min() {
        sung::TimeChecker sw;
        sw.set_min();
        if (sw.elapsed() < 0) {
            std::cout
                << "Test `test_min` failed: Elapsed time of min is less than 0"
                << std::endl;
            return TEST_RESULT_FAIL_INVALID_MIN;
        }

        return TEST_RESULT_SUCCESS;
    }

    TestResult test_sleep(std::function<void(double)> sleep_func) {
        constexpr double EPSILON = 0.001;
        constexpr double SLEEP_SEC = 1;

        sung::TimeChecker sw;
        sleep_func(SLEEP_SEC);
        const auto elapsed = sw.elapsed();
        const auto redisual = elapsed - SLEEP_SEC;

        if (redisual < -EPSILON || redisual > EPSILON) {
            std::cout << "Test `test_sleep` failed"
                      << "\n    elapsed: " << elapsed
                      << "\n    redisual: " << redisual
                      << "\n    redisual rate: " << redisual / SLEEP_SEC
                      << std::endl;
            return TEST_RESULT_FAIL_SLEEP;
        } else {
            std::cout << "Test `test_sleep` passed"
                      << "\n    elapsed: " << elapsed
                      << "\n    redisual: " << redisual
                      << "\n    redisual rate: " << redisual / SLEEP_SEC
                      << std::endl;
            return TEST_RESULT_SUCCESS;
        }
    }

}  // namespace


int main() {
    auto result = test_timepoint();
    if (TEST_RESULT_SUCCESS != result)
        return result;

    result = test_min();
    if (TEST_RESULT_SUCCESS != result)
        return result;

    result = test_sleep(sung::sleep_loop);
    if (TEST_RESULT_SUCCESS != result)
        return result;

    result = test_sleep([](double sec) { sung::sleep_hybrid(sec, 0.5); });
    if (TEST_RESULT_SUCCESS != result)
        return result;

    return 0;
}
