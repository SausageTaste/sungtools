#pragma once

#include <array>
#include <chrono>

#include "sung/basic/input_enum.h"
#include "sung/basic/optional.hpp"


namespace sung { namespace key {

    struct Event {
        using Clock_t = std::chrono::steady_clock;

        Clock_t::time_point tp_ = Clock_t::now();
        KeyAction action_ = KeyAction::eoe;
        KeyCode key_ = KeyCode::eoe;
    };


    class EventAnalyzer {

    private:
        using Clock_t = Event::Clock_t;

        struct KeyState {
            Clock_t::time_point tp_ = Clock_t::now();
            bool pressed = false;
        };

    public:
        void notify(const Event& e);
        bool is_pressed(KeyCode key) const;
        sung::Optional<Clock_t::time_point> get_timepoint(KeyCode key) const;

    private:
        static size_t convert_key_to_index(const KeyCode key);

        constexpr static auto KEYSPEC_SIZE = static_cast<size_t>(KeyCode::eoe);
        std::array<KeyState, KEYSPEC_SIZE> states_;
    };

}}  // namespace sung::key


namespace sung { namespace mouse {

    struct Event {
        using Clock_t = std::chrono::steady_clock;

        Clock_t::time_point timepoint_ = Clock_t::now();
        MouseAction action_ = MouseAction::eoe;
        MouseBtn button_ = MouseBtn::eoe;
        int32_t btn_id_ = -1;
        double xpos_ = 0;
        double ypos_ = 0;
    };

}}  // namespace sung::mouse
