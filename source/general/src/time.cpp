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


// MonotonicRealtimeClock
namespace sung {

    double MonotonicRealtimeClock::calc_dur_sec(tp_t start, tp_t end) {
        return ::dur_cast(end - start);
    }

}  // namespace sung


// ManualNumericClock
namespace sung {

    // Returns true if the time rewinded
    bool ManualNumericClock::add(double value) {
        cur_time_ += value;
        return value < 0;
    }

    bool ManualNumericClock::set(double value) {
        const auto out = cur_time_ > value;
        cur_time_ = value;
        return out;
    }

    bool ManualNumericClock::set_max() {
        return this->set(std::numeric_limits<float>::max());
    }

    bool ManualNumericClock::set_min() { return this->set(0); }

}  // namespace sung
