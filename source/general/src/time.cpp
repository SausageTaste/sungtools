#define _CRT_SECURE_NO_WARNINGS

#include "sung/general/time.hpp"

#include <array>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <thread>

#include "sung/general/os_detect.hpp"


namespace {

    // Duration cast
    template <typename TRep, typename TPeriod>
    double dur_cast(std::chrono::duration<TRep, TPeriod> dur) {
        namespace chr = std::chrono;
        return chr::duration_cast<chr::duration<double>>(dur).count();
    }

    std::string make_sec_frac_str(
        double unix_time, bool remove_tail_zero, size_t digits
    ) {
        std::string out;  // RVO!!!

        std::stringstream ss;
        ss << std::fixed << std::fmod(unix_time, 1.0);
        out = ss.str().substr(1, digits + 1);

        if (remove_tail_zero) {
            while (true) {
                if (out.empty()) {
                    break;
                } else if (out.back() == '0') {
                    out.pop_back();
                } else if (out.back() == '.') {
                    out.clear();
                    break;
                } else {
                    break;
                }
            }
        }

        return out;
    }

}  // namespace


namespace sung { namespace backend {

    uint64_t get_itime_unix() {
        // static_assert(sizeof(time_t) >= 8, "time_t is too small");
        static_assert(sizeof(std::time_t) == sizeof(time_t), "time_t differs");

        return std::time(nullptr);
    }

    double get_time_unix_time_t() {
        return static_cast<double>(std::time(nullptr));
    }

    double get_time_unix_chrono() {
        const auto now = std::chrono::system_clock::now();
        return ::dur_cast(now.time_since_epoch());
    }

}}  // namespace sung::backend


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

    std::string get_time_iso_utc(
        bool milisec, bool remove_tail_zero, size_t digits
    ) {
        const auto uti = get_time_unix();
        const auto tit = static_cast<time_t>(uti);
        const auto tmm = *std::gmtime(&tit);

        std::ostringstream ss;
        ss << std::put_time(&tmm, "%FT%T");
        if (milisec)
            ss << ::make_sec_frac_str(uti, remove_tail_zero, digits);
        ss << std::put_time(&tmm, "Z");
        return ss.str();
    }

    std::string get_time_iso_local(
        bool milisec, bool remove_tail_zero, size_t digits
    ) {
        const auto uti = get_time_unix();
        const auto tit = static_cast<time_t>(uti);
        const auto tmm = *std::localtime(&tit);

        std::ostringstream ss;
        ss << std::put_time(&tmm, "%FT%T");
        if (milisec)
            ss << ::make_sec_frac_str(uti, remove_tail_zero, digits);
        ss << std::put_time(&tmm, "%z");

        auto str = ss.str();
        str.insert(str.size() - 2, ":");
        return str;
    }

    std::string get_time_iso_local_slug(
        bool milisec, bool remove_tail_zero, size_t digits
    ) {
        const auto uti = get_time_unix();
        const auto tit = static_cast<time_t>(uti);
        const auto tmm = *std::localtime(&tit);

        std::ostringstream ss;
        ss << std::put_time(&tmm, "%Y%m%dT%H%M%S");
        if (milisec)
            ss << ::make_sec_frac_str(uti, remove_tail_zero, digits);
        ss << std::put_time(&tmm, "%z");
        return ss.str();
    }

}  // namespace sung


// MonotonicRealtimeClock
namespace sung {

    MonotonicRealtimeClock::tp_t MonotonicRealtimeClock::now() {
        return clock_t::now();
    }

    MonotonicRealtimeClock::tp_t MonotonicRealtimeClock::min_val() {
        return tp_t{};
    }

    double MonotonicRealtimeClock::calc_dur_sec(tp_t start, tp_t end) {
        return ::dur_cast(end - start);
    }

}  // namespace sung


// ManualNumericClock
namespace sung {

    ManualNumericClock::tp_t ManualNumericClock::now() const {
        return cur_time_;
    }

    ManualNumericClock::tp_t ManualNumericClock::min_val() { return 0; }

    ManualNumericClock::tp_t ManualNumericClock::calc_dur_sec(
        tp_t start, tp_t end
    ) {
        return end - start;
    }

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
