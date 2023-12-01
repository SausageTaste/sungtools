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
        void set_signal(bool v);

        // Methods with names starting with `check_` will update the internal state.
        Type check_edge();
        bool check_rising() { return Type::rising == this->check_edge(); }
        bool check_falling() { return Type::falling == this->check_edge(); }
        bool check_any_edge() { return Type::none != this->check_edge(); }
        void check() { last_checked_signal_ = signal_; }

    private:
        Type edge_type() const;

        bool signal_ = false;
        bool last_checked_signal_ = false;

    };


    class RetriggerableMonostableMultivibrator {

    public:
        RetriggerableMonostableMultivibrator();
        void notify_signal(bool value);
        bool out_signal(double tolerance_sec);

    private:
        TimeChecker timer_;
        bool signal_once_ = false;  // Ensure at least one true signal to be reported

    };


    class LongPressDetector {

    public:
        enum class Type { none, short_press, long_press };

        Type notify_signal(bool pressed, double threshold_sec);

    private:
        EdgeDetector edge_detector_;
        TimeChecker last_state_changed_;
        bool consumed_ = false;

    };


    class PulseResponseFuture {

    public:
        void add_signal(double delay_sec) {
            records_.push_back({ TimeChecker{}, delay_sec });
        }

        bool get_pulse() {
            for (auto it = records_.begin(); it != records_.end(); ++it) {
                if (it->timer_.elapsed() >= it->delay_sec_) {
                    records_.erase(it);
                    return true;
                }
            }
            return false;
        }

    private:
        struct Record {
            TimeChecker timer_;
            double delay_sec_;
        };

        std::vector<Record> records_;

    };

}
