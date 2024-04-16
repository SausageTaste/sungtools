#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <chrono>
#include <string>


namespace sung {

    void sleep_naive(double seconds);
    void sleep_loop(double seconds);
    void sleep_hybrid(double seconds, double proportion = 0.5);


    class CalenderTime {

    public:
        using Clock_t = std::chrono::system_clock;

        struct HumanReadable {
            int year_;
            int month_;
            int day_;
            int hour_;
            int minute_;
            int second_;
        };

        CalenderTime() = default;

        bool operator==(const CalenderTime& rhs) const;
        bool operator!=(const CalenderTime& rhs) const;

        static CalenderTime from_now();
        static CalenderTime from_total_sec(double total_seconds);
        static CalenderTime from_time_point(Clock_t::time_point time_point);
        static CalenderTime from_time_t(time_t time);

        // Since epoch
        auto to_total_seconds() const -> double;
        auto to_time_point() const -> Clock_t::time_point;
        auto to_time_t() const -> time_t;

        std::string make_locale_text() const;
        std::string make_sortable_text(bool utc) const;

        HumanReadable local_time() const;
        HumanReadable utc_time() const;

    private:
        CalenderTime(Clock_t::time_point value) : value_(value) {}

        Clock_t::time_point value_;
    };


    class IClock {

    public:
        virtual ~IClock() = default;

        virtual double elapsed() const = 0;

        virtual void set_min() = 0;
        virtual void check() = 0;
        virtual double check_get_elapsed() = 0;
        virtual bool check_if_elapsed(double seconds) = 0;
    };


    class MonotonicClock : public IClock {

    public:
        double elapsed() const override final;

        void set_min() override final;
        void check() override final;
        double check_get_elapsed() override final;
        bool check_if_elapsed(double seconds) override final;

    private:
        using Clock_t = std::chrono::steady_clock;
        Clock_t::time_point last_checked_ = Clock_t::now();
    };


    class ManualClock : public IClock {

    public:
        double elapsed() const override final;

        void set_min() override final;
        void check() override final;
        double check_get_elapsed() override final;
        bool check_if_elapsed(double seconds) override final;

        double last_added_value() const;

        void add(double value);
        void set(double value);
        void set_max();

    private:
        double accum_ = 0;
        double last_added_value_ = 0;
    };

}  // namespace sung
