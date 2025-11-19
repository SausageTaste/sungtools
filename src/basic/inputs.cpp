#include "sung/basic/inputs.hpp"


namespace sung {

    void KeyEventStates::notify(const KeyEvent& e) {
        const auto index = static_cast<size_t>(e.key_);
        if (index >= states_.size())
            return;

        states_[index].timepoint_ = e.timepoint_;
        states_[index].pressed = (e.action_ == KeyAction::down);
    }

    bool KeyEventStates::is_pressed(KeyCode key) const {
        const auto index = static_cast<size_t>(key);
        if (index >= states_.size())
            return false;

        return states_[index].pressed;
    }

    sung::Optional<EventTimePoint> KeyEventStates::get_timepoint(
        KeyCode key
    ) const {
        const auto index = static_cast<size_t>(key);
        if (index >= states_.size())
            return sung::nullopt;

        return states_[index].timepoint_;
    }

}  // namespace sung
