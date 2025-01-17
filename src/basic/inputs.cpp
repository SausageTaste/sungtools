#include "sung/basic/inputs.hpp"


namespace sung { namespace key {

    void EventAnalyzer::notify(const Event& e) {
        const auto index = static_cast<size_t>(e.key_);
        if (index >= states_.size())
            return;

        states_[index].tp_ = e.tp_;
        states_[index].pressed = (e.action_ == key::ActionType::down);
    }

    bool EventAnalyzer::is_pressed(KeyCode key) const {
        const auto index = static_cast<size_t>(key);
        if (index >= states_.size())
            return false;

        return states_[index].pressed;
    }

    sung::Optional<EventAnalyzer::Clock_t::time_point>
    EventAnalyzer::get_timepoint(KeyCode key) const {
        const auto index = static_cast<size_t>(key);
        if (index >= states_.size())
            return sung::nullopt;

        return states_[index].tp_;
    }

    size_t EventAnalyzer::convert_key_to_index(const KeyCode key) {
        return static_cast<size_t>(key);
    }

}}  // namespace sung::key
