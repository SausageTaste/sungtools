#pragma once

#include <chrono>


namespace sung {

    void sleep_naive(double seconds);
    void sleep_loop(double seconds);
    void sleep_hybrid(double seconds, double proportion = 0.5);


    class ITimer {

    public:
        virtual ~ITimer() = default;

        virtual double elapsed() const = 0;

        virtual void set_min() = 0;
        virtual void check() = 0;
        virtual double check_get_elapsed() = 0;
        virtual bool check_if_elapsed(double seconds) = 0;

    };


    /*
    This class uses monotonic clock.
    */
    class TimeChecker : public ITimer {

    public:
        double elapsed() const override final;

        void set_min() override final;
        void check() override final;
        double check_get_elapsed() override final;
        bool check_if_elapsed(double seconds) override final;

    private:
        using Clock_t = std::chrono::steady_clock;

        static double duration_cast(Clock_t::duration duration);

        Clock_t::time_point last_checked_ = Clock_t::now();

    };


    class TimeAccumulator : public ITimer {

    public:
        double elapsed() const override final;

        void set_min() override final;
        void check() override final;
        double check_get_elapsed() override final;
        bool check_if_elapsed(double seconds) override final;

        void add(double value);
        void set_max();

    private:
        double accum_ = 0;
        double lasted_added_value_ = 0;

    };

}
