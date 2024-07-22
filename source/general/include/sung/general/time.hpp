#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <chrono>
#include <string>

#ifdef __cpp_lib_format
    #include <format>
#endif


namespace sung {

    void sleep_naive(double seconds);
    void sleep_loop(double seconds);
    void sleep_hybrid(double seconds, double proportion = 0.5);


    std::string get_cur_time_iso_utc_strftime();

    inline std::string get_cur_time_iso_utc() {
#ifdef __cpp_lib_format
        const auto now = std::chrono::system_clock::now();
        return std::format("{:%FT%TZ}", now);
#else
        return get_cur_time_iso_utc_strftime();
#endif
    }


    class MonotonicRealtimeClock {

    public:
        using clock_t = std::chrono::steady_clock;
        using tp_t = clock_t::time_point;

        static tp_t now() { return clock_t::now(); }
        static tp_t min_val() { return tp_t{}; }
        static double calc_dur_sec(tp_t start, tp_t end);
    };


    class ManualNumericClock {

    public:
        using tp_t = double;

        tp_t now() const { return cur_time_; }

        static tp_t min_val() { return 0; }
        static tp_t calc_dur_sec(tp_t start, tp_t end) { return end - start; }

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

        TClock clock_{};
        typename TClock::tp_t last_checked_{};
    };


    using MonotonicRealtimeTimer = TTimer<MonotonicRealtimeClock>;
    using ManualNumericTimer = TTimer<ManualNumericClock>;

}  // namespace sung
