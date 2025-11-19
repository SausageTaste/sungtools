#pragma once

#include <array>
#include <chrono>

#include "sung/basic/input_enum.h"
#include "sung/basic/optional.hpp"


namespace sung {

    using EventClock = std::chrono::steady_clock;
    using EventTimePoint = EventClock::time_point;


    struct KeyEvent {
        EventClock::time_point timepoint_ = EventClock::now();
        KeyAction action_ = KeyAction::eoe;
        KeyCode key_ = KeyCode::eoe;
    };


    class KeyEventStates {

    public:
        void notify(const KeyEvent& e);
        bool is_pressed(KeyCode key) const;
        sung::Optional<EventTimePoint> get_timepoint(KeyCode key) const;

    private:
        struct KeyState {
            EventTimePoint timepoint_ = EventClock::now();
            bool pressed = false;
        };

        constexpr static auto KEYSPEC_SIZE = static_cast<size_t>(KeyCode::eoe);
        std::array<KeyState, KEYSPEC_SIZE> states_;
    };


    struct MouseEvent {
        double x_ = 0;
        double y_ = 0;
        EventTimePoint timepoint_ = EventClock::now();
        MouseAction action_ = MouseAction::eoe;
        MouseBtn button_ = MouseBtn::eoe;
        int32_t btn_id_ = -1;
    };

    constexpr auto size = sizeof(MouseEvent);
    constexpr auto offset = offsetof(MouseEvent, btn_id_);

}  // namespace sung
