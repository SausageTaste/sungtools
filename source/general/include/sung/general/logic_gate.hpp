#pragma once

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

}
