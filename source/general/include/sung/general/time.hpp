#pragma once

#include <chrono>


namespace sung {

    /*
    This class uses monotonic clock.
    */
    class TimeChecker {

    public:
        double elapsed() const;

        void set_min();
        void check();
        double check_get_elapsed();

    private:
        using Clock_t = std::chrono::steady_clock;

        static double duration_cast(Clock_t::duration duration);

        Clock_t::time_point last_checked_ = Clock_t::now();

    };

}
