#include "sung/general/time.hpp"

#include <thread>


namespace sung {

    void sleep_naive(double seconds) {
        std::this_thread::sleep_for(std::chrono::duration<double>(seconds));
    }

    void sleep_loop(double seconds) {
        using clock_t = std::chrono::steady_clock;
        using duration_t = std::chrono::duration<double>;
        constexpr static duration_t MIN_SLEEP_DURATION(0);

        const auto start = clock_t::now();
        while (duration_t(clock_t::now() - start).count() < seconds) {
            std::this_thread::sleep_for(MIN_SLEEP_DURATION);
        }
    }

    void sleep_hybrid(double seconds, double proportion) {
        using clock_t = std::chrono::steady_clock;
        using duration_t = std::chrono::duration<double>;
        constexpr static duration_t MIN_SLEEP_DURATION(0);

        const auto start = clock_t::now();
        std::this_thread::sleep_for(duration_t(seconds * proportion));
        while (duration_t(clock_t::now() - start).count() < seconds) {
            std::this_thread::sleep_for(MIN_SLEEP_DURATION);
        }
    }

}  // namespace sung


// TimeChecker
namespace sung {

    double TimeChecker::elapsed() const {
        return this->duration_cast(Clock_t::now() - last_checked_);
    }

    void TimeChecker::set_min() { last_checked_ = Clock_t::time_point{}; }

    void TimeChecker::check() { last_checked_ = Clock_t::now(); }

    double TimeChecker::check_get_elapsed() {
        const auto now = Clock_t::now();
        const auto elapsed = this->duration_cast(now - last_checked_);
        last_checked_ = now;
        return elapsed;
    }

    bool TimeChecker::check_if_elapsed(double seconds) {
        const auto now = Clock_t::now();
        const auto elapsed = this->duration_cast(now - last_checked_);
        if (elapsed >= seconds) {
            last_checked_ = now;
            return true;
        }
        return false;
    }

    double TimeChecker::duration_cast(Clock_t::duration duration) {
        namespace chr = std::chrono;
        return chr::duration_cast<chr::duration<double>>(duration).count();
    }

}  // namespace sung


// TimeAccumulator
namespace sung {

    double TimeAccumulator::elapsed() const { return accum_; }

    void TimeAccumulator::set_min() {
        accum_ = 0;
        lasted_added_value_ = 0;
    }

    void TimeAccumulator::check() { this->set_min(); }

    double TimeAccumulator::check_get_elapsed() {
        const auto result = accum_;
        this->set_min();
        return result;
    }

    bool TimeAccumulator::check_if_elapsed(double seconds) {
        if (accum_ >= seconds) {
            this->set_min();
            return true;
        }
        return false;
    }

    void TimeAccumulator::add(double value) {
        accum_ += value;
        lasted_added_value_ = value;
    }

    void TimeAccumulator::set_max() {
        constexpr double MAX_VALUE = 9999999999999;  // Such random number
        lasted_added_value_ = MAX_VALUE - accum_;
        accum_ = MAX_VALUE;
    }

}  // namespace sung
