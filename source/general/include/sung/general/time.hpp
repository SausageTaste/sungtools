#pragma once

#include <chrono>
#include <string>

#include "sung/general/os_detect.hpp"

#if SUNG__cplusplus >= 202002L
    #define SUNG_UNIX_CHRONO
#elif defined(SUNG_OS_WINDOWS)
    #define SUNG_UNIX_CHRONO
#endif


namespace sung { namespace backend {

    uint64_t get_itime_unix();

    double get_time_unix_time_t();
    double get_time_unix_chrono();

}}  // namespace sung::backend


namespace sung {

    void sleep_naive(double seconds);
    void sleep_loop(double seconds);
    void sleep_hybrid(double seconds, double proportion = 0.5);


    inline double get_time_unix() {
#ifdef SUNG_UNIX_CHRONO
        return backend::get_time_unix_chrono();
#else
        return backend::get_time_unix_time_t();
#endif
    }

    std::string get_time_iso_utc(
        bool milisec = false, bool remove_tail_zero = false, size_t digits = 3
    );
    std::string get_time_iso_local(
        bool milisec = false, bool remove_tail_zero = false, size_t digits = 3
    );
    std::string get_time_iso_local_slug(
        bool milisec = false, bool remove_tail_zero = false, size_t digits = 3
    );


    class MonotonicRealtimeClock {

    public:
        using clock_t = std::chrono::steady_clock;
        using tp_t = clock_t::time_point;

        static tp_t now();
        static tp_t min_val();
        static double calc_dur_sec(tp_t start, tp_t end);
    };


    class ManualNumericClock {

    public:
        using tp_t = double;

        tp_t now() const;
        static tp_t min_val();
        static tp_t calc_dur_sec(tp_t start, tp_t end);

        // Returns true if the time rewinded
        bool add(double value);
        bool set(tp_t value);
        bool set_max();
        bool set_min();

    private:
        tp_t cur_time_ = 0;
    };


    template <typename TClock>
    class TTimer {

    public:
        TTimer() { last_checked_ = clock_.now(); }

        double elapsed() const {
            return clock_.calc_dur_sec(last_checked_, clock_.now());
        }

        bool has_elapsed(const double sec) const {
            return this->elapsed() >= sec;
        }

        auto& last_checked() const { return last_checked_; }

        void set_min() { last_checked_ = clock_.min_val(); }

        void check() { last_checked_ = clock_.now(); }

        double check_get_elapsed() {
            const auto now = clock_.now();
            const auto elapsed = clock_.calc_dur_sec(last_checked_, now);
            last_checked_ = now;
            return elapsed;
        }

        bool check_if_elapsed(double seconds) {
            const auto now = clock_.now();
            const auto elapsed = clock_.calc_dur_sec(last_checked_, now);
            if (elapsed >= seconds) {
                last_checked_ = now;
                return true;
            }
            return false;
        }

        TClock& clock() { return clock_; }
        const TClock& clock() const { return clock_; }

    private:
        TClock clock_{};
        typename TClock::tp_t last_checked_{};
    };


    using MonotonicRealtimeTimer = TTimer<MonotonicRealtimeClock>;
    using ManualNumericTimer = TTimer<ManualNumericClock>;

}  // namespace sung
