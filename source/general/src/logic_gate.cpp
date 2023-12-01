#include "sung/general/logic_gate.hpp"


// EdgeDetector
namespace sung {

    bool EdgeDetector::signal() const {
        return signal_;
    }

    void EdgeDetector::set_signal(bool v) {
        signal_ = v;
    }

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

}


// RetriggerableMonostableMultivibrator
namespace sung {

    RetriggerableMonostableMultivibrator::RetriggerableMonostableMultivibrator() {
        timer_.set_min();
    }

    void RetriggerableMonostableMultivibrator::notify_signal(bool value) {
        if (value) {
            timer_.check();
            signal_once_ = true;
        }
    }

    bool RetriggerableMonostableMultivibrator::out_signal(double tolerance_sec) {
        if (signal_once_) {
            signal_once_ = false;
            return true;
        }

        return timer_.elapsed() <= tolerance_sec;
    }

}


// LongPressDetector
namespace sung {

    LongPressDetector::Type LongPressDetector::notify_signal(bool pressed, double threshold_sec) {
        edge_detector_.set_signal(pressed);
        const auto edge = edge_detector_.check_edge();

        switch (edge) {
        case EdgeDetector::Type::rising:
            last_state_changed_.check();
            break;
        case EdgeDetector::Type::falling:
            if (!consumed_ && last_state_changed_.elapsed() < threshold_sec) {
                last_state_changed_.check();
                consumed_ = false;
                return Type::short_press;
            }
            else {
                last_state_changed_.check();
                consumed_ = false;
            }
            break;
        }

        if (!pressed)
            return Type::none;
        if (consumed_)
            return Type::none;

        if (last_state_changed_.elapsed() >= threshold_sec) {
            consumed_ = true;
            return Type::long_press;
        }

        return Type::none;
    }

}
