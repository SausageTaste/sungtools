#pragma once

#include <vector>

#include "sung/general/time.hpp"


namespace sung {

    /*
    Call `notify_signal` methods to notify the signal to the detector. The
    signal is considered to be in the rising state when the input signal changes
    from false to true. The signal is considered to be in the falling state when
    the input signal changes from true to false. The signal is considered to be
    in the none state when the input signal does not change.

    Call one of the `check_*` methods periodically to poll the type of the
    signal edge. Those methods with `check_` prefix will update the internal
    state so you must not call it more than once in a loop.
    */
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

        // This to to discard any edges currently detected
        void check() { last_checked_signal_ = signal_; }

        Type notify_check_edge(bool v) {
            this->notify_signal(v);
            return this->check_edge();
        }
        void reset() {
            signal_ = false;
            last_checked_signal_ = false;
        }

    private:
        Type edge_type() const;

        bool signal_ = false;
        bool last_checked_signal_ = false;
    };


    /**
     * Monostable Multivibrator (MMV) is an electronic circuit that has two
     * states: stable and unstable. When the input signal changes, the MMV will
     * change its state to unstable for a certain period of time. After that, it
     * will return to the stable state. True is the unstable state, and false is
     * the stable state.
     *
     * This class is a retriggerable MMV. It will stay in the unstable state as
     * long as the input signal is true. If the input signal is false, the MMV
     * will return to the stable state after a certain period of time.
     *
     * You can search for "monostable multivibrator" on the internet to get more
     * information.
     *
     * You may use `sung::MonotonicRealtimeTimer` for template parameter
     * `TTimer` if your use case needs a real time clock. But if you need to
     * manually control the timer, you may use `sung::ManualNumericTimer`
     * instead, in which case you need to manually control the timer. You can
     * access the timer object using the `timer()` method.
     */
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
    using RetriggerableMMV = RetriggerableMonostableMultivibrator<TTimer>;


    /*
    This is same as `RetriggerableMonostableMultivibrator` but with a tolerance
    parameter embedded in the class.
    */
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


    template <typename TTimer>
    using RetriggerableMMVCompact =
        RetriggerableMonostableMultivibratorCompact<TTimer>;


    class LongPressDetector {

    public:
        enum class Type { none, short_press, long_press };

        // Call it periodically to poll the signal.
        Type notify_poll(bool pressed, double threshold_sec);

    private:
        EdgeDetector edge_detector_;
        MonotonicRealtimeTimer last_state_changed_;
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
