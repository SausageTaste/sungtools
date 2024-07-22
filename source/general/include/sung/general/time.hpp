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

        void add(double value);
        void set(double value);
        void set_max();

    private:
        double cur_time_ = 0;
        double last_checked_ = 0;
    };

}  // namespace sung
