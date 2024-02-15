#pragma once

#include <chrono>


namespace sung {

    void sleep_naive(double seconds);
    void sleep_loop(double seconds);
    void sleep_hybrid(double seconds, double proportion = 0.5);


    /*
    This class uses monotonic clock.
    */
    class TimeChecker {

    public:
        double elapsed() const;

        void set_min();
        void check();
        double check_get_elapsed();
        bool check_if_elapsed(double seconds);

    private:
        using Clock_t = std::chrono::steady_clock;

        static double duration_cast(Clock_t::duration duration);

        Clock_t::time_point last_checked_ = Clock_t::now();

    };


    class TimeAccumulator {

    public:
        double elapsed() const;

        void add(double value);
        void set_max();
        void set_min();
        void check();
        double check_get_elapsed();
        double check_if_elapsed(double seconds);

    private:
        double accum_ = 0;
        double lasted_added_value_ = 0;

    };

}
