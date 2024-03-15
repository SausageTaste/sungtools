#pragma once

#include <vector>

#include "sung/general/time.hpp"


namespace sung {

    class EdgeDetector {

    public:
        enum class Type {
            none,
            rising,
            falling,
        };

        // Input signal
        bool signal() const;
        void notify_signal(bool v);

        // Methods with names starting with `check_` will update the internal
        // state. Call one of these periodically to poll the signal.
        Type check_edge();
        bool check_rising() { return Type::rising == this->check_edge(); }
        bool check_falling() { return Type::falling == this->check_edge(); }
        bool check_any_edge() { return Type::none != this->check_edge(); }
        void check() { last_checked_signal_ = signal_; }

        Type notify_check_edge(bool v) {
            this->notify_signal(v);
            return this->check_edge();
        }

    private:
        Type edge_type() const;

        bool signal_ = false;
        bool last_checked_signal_ = false;
    };


    template <typename TTimer>
    class RetriggerableMonostableMultivibrator {

    public:
        RetriggerableMonostableMultivibrator() { timer_.set_min(); }

        void notify_signal(bool value) {
            if (value) {
                timer_.check();
                signal_once_ = true;
            }
        }

        bool poll_signal(double tolerance_sec) {
            if (signal_once_) {
                signal_once_ = false;
                return true;
            }

            if (timer_.elapsed() <= tolerance_sec)
                return true;

            // To prevent false positive after tolerance time increased
            timer_.set_min();
            return false;
        }

        TTimer& timer() { return timer_; }

    private:
        TTimer timer_;

        // Ensure at least one true signal to be reported
        bool signal_once_ = false;
    };


    template <typename TTimer>
    class RetriggerableMonostableMultivibratorCompact {

    public:
        RetriggerableMonostableMultivibratorCompact(double tolerance_sec)
            : tolerance_sec_(tolerance_sec) {}
        void notify_signal(bool value) { rmm_.notify_signal(value); }
        bool poll_signal(double tolerance_sec) {
            return rmm_.poll_signal(tolerance_sec);
        }

        double tolerance() const { return tolerance_sec_; }
        void set_tolerance(double tolerance_sec) {
            tolerance_sec_ = tolerance_sec;
        }

        TTimer& timer() { return rmm_.timer_; }

    private:
        RetriggerableMonostableMultivibrator<TTimer> rmm_;

        double tolerance_sec_ = 0;
    };


    class LongPressDetector {

    public:
        enum class Type { none, short_press, long_press };

        // Call it periodically to poll the signal.
        Type notify_poll(bool pressed, double threshold_sec);

    private:
        EdgeDetector edge_detector_;
        TimeChecker last_state_changed_;
        bool consumed_ = false;
    };


    class PulseResponseFuture {

    public:
        void add_future(double delay_sec);
        bool poll_pulse();

    private:
        class Record;
        std::vector<Record> records_;
    };

}  // namespace sung
