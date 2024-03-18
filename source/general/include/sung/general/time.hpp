#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <chrono>
#include <string>


namespace sung {

    void sleep_naive(double seconds);
    void sleep_loop(double seconds);
    void sleep_hybrid(double seconds, double proportion = 0.5);


    class TimePoint {

    public:
        using Clock_t = std::chrono::system_clock;

        struct YearMonthDay {
            int year_;
            int month_;
            int day_;
        };

        struct HourMinuteSecond {
            int hour_;
            int minute_;
            int second_;
        };

        struct YearMonthDayHourMinuteSecond {
            int year_;
            int month_;
            int day_;
            int hour_;
            int minute_;
            int second_;
        };

        static TimePoint from_now();
        static TimePoint from_total_sec(double total_seconds);
        static TimePoint from_time_point(Clock_t::time_point time_point);
        static TimePoint from_time_t(time_t time);

        // Since epoch
        auto to_total_seconds() const -> double;
        auto to_time_point() const -> Clock_t::time_point;
        auto to_time_t() const -> time_t;
        auto to_datetime_text() const -> std::string;

        YearMonthDayHourMinuteSecond local_time() const;
        YearMonthDayHourMinuteSecond utc_time() const;

    private:
        TimePoint(Clock_t::time_point value) : value_(value) {}

        Clock_t::time_point value_;
    };


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

}  // namespace sung
