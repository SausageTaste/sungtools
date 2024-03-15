#include "sung/general/logic_gate.hpp"


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
        bool pressed, double threshold
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
        }

        if (consumed_)
            return Type::none;
        if (pressed && last_state_changed_.elapsed() >= threshold) {
            consumed_ = true;
            return Type::long_press;
        }

        return Type::none;
    }

}  // namespace sung


// PulseResponseFuture
namespace sung {

    class PulseResponseFuture::Record {

    public:
        explicit Record(double delay_sec) : delay_sec_(delay_sec) {}

        bool expired() const { return timer_.elapsed() >= delay_sec_; }

    private:
        TimeChecker timer_;
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
