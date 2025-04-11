#include "sung/basic/logic_gate.hpp"


// EdgeDetector
namespace sung {

    bool EdgeDetector::signal() const { return signal_; }

    void EdgeDetector::notify_signal(bool v) { signal_ = v; }

    EdgeDetector::Type EdgeDetector::check_edge() {
        const auto output = this->edge_type();
        this->check();
        return output;
    }

    EdgeDetector::Type EdgeDetector::edge_type() const {
        if (signal_ != last_checked_signal_)
            return signal_ ? Type::rising : Type::falling;
        else
            return Type::none;
    }

}  // namespace sung


// LongPressDetector
namespace sung {

    LongPressDetector::Type LongPressDetector::notify_poll(
        bool pressed, double threshold, bool long_press_repeat
    ) {
        switch (edge_detector_.notify_check_edge(pressed)) {
            case EdgeDetector::Type::rising:
                last_state_changed_.check();
                break;
            case EdgeDetector::Type::falling:
                if (!consumed_ && last_state_changed_.elapsed() < threshold) {
                    last_state_changed_.check();
                    consumed_ = false;
                    return Type::short_press;
                } else {
                    last_state_changed_.check();
                    consumed_ = false;
                    return Type::none;
                }
                break;
            case EdgeDetector::Type::none:
                break;
        }

        if (!long_press_repeat && consumed_)
            return Type::none;
        if (pressed && last_state_changed_.elapsed() >= threshold) {
            consumed_ = true;
            return Type::long_press;
        }

        return Type::none;
    }

}  // namespace sung


// RepeatedPulseGenerator
namespace sung {

    void RepeatedPulseGenerator::notify(bool signal) {
        active_ = signal;
        edge_.notify_signal(active_);
    }

    bool RepeatedPulseGenerator::poll(double pulse_interval) {
        const auto edge = edge_.check_edge();

        switch (edge) {
            case EdgeDetector::Type::rising:
                timer_.check();
                return true;
            case EdgeDetector::Type::falling:
                return false;
        }

        if (active_)
            return timer_.check_if_elapsed(pulse_interval);

        return false;
    }

    void RepeatedPulseGenerator::discard() {
        edge_.check();
        if (active_)
            timer_.check();
    }

}  // namespace sung


// PulseResponseFuture
namespace sung {

    class PulseResponseFuture::Record {

    public:
        explicit Record(double delay_sec) : delay_sec_(delay_sec) {}

        bool expired() const { return timer_.elapsed() >= delay_sec_; }

    private:
        MonotonicRealtimeTimer timer_;
        double delay_sec_;
    };


    void PulseResponseFuture::add_future(double delay_sec) {
        records_.emplace_back(delay_sec);
    }

    bool PulseResponseFuture::poll_pulse() {
        for (auto it = records_.begin(); it != records_.end(); ++it) {
            if (it->expired()) {
                records_.erase(it);
                return true;
            }
        }
        return false;
    }

}  // namespace sung
