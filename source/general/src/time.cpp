#include "sung/general/time.hpp"

#include <array>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <thread>

#include "sung/general/os_detect.hpp"


namespace {

    // Duration cast
    double dur_cast(std::chrono::steady_clock::duration dur) {
        namespace chr = std::chrono;
        return chr::duration_cast<chr::duration<double>>(dur).count();
    }

}  // namespace


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


    std::string get_cur_time_iso_utc_strftime() {
        std::string out;

        time_t now;
        time(&now);
        std::array<char, 128> buf;
        auto res = strftime(buf.data(), buf.size(), "%FT%TZ", gmtime(&now));
        // strftime(buf, sizeof buf, "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));

        if (0 != res)
            out.assign(buf.data(), res);

        return out;
    }

}  // namespace sung


// MonotonicClock
namespace sung {

    double MonotonicClock::elapsed() const {
        return ::dur_cast(Clock_t::now() - last_checked_);
    }

    void MonotonicClock::set_min() { last_checked_ = Clock_t::time_point{}; }

    void MonotonicClock::check() { last_checked_ = Clock_t::now(); }

    double MonotonicClock::check_get_elapsed() {
        const auto now = Clock_t::now();
        const auto elapsed = ::dur_cast(now - last_checked_);
        last_checked_ = now;
        return elapsed;
    }

    bool MonotonicClock::check_if_elapsed(double seconds) {
        const auto now = Clock_t::now();
        const auto elapsed = ::dur_cast(now - last_checked_);
        if (elapsed >= seconds) {
            last_checked_ = now;
            return true;
        }
        return false;
    }

}  // namespace sung


// ManualClock
namespace sung {

    double ManualClock::elapsed() const { return cur_time_ - last_checked_; }

    void ManualClock::set_min() { this->set(0); }

    void ManualClock::check() { last_checked_ = cur_time_; }

    double ManualClock::check_get_elapsed() {
        const auto result = this->elapsed();
        this->check();
        return result;
    }

    bool ManualClock::check_if_elapsed(double seconds) {
        if (this->elapsed() >= seconds) {
            this->check();
            return true;
        }
        return false;
    }

    void ManualClock::add(double value) { cur_time_ += value; }

    void ManualClock::set(double value) { cur_time_ = value; }

    void ManualClock::set_max() {
        this->set(999999999999999);  // Such random number
    }

}  // namespace sung
