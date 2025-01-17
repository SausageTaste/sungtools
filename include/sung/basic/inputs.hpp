#pragma once

#include <array>
#include <chrono>

#include "sung/basic/optional.hpp"


namespace sung { namespace key {

    enum class ActionType { down, up };

    // clang-format off
    enum class KeyCode {
        /* Alphabets */
        a, b, c, d, e, f, g, h, i, j, k, l, m,
        n, o, p, q, r, s, t, u, v, w, x, y, z,
        /* Horizontal numbers */
        n0, n1, n2, n3, n4, n5, n6, n7, n8, n9,
        /* Misc in keyboard main area */
        backquote, minus, equal, lbracket, rbracket, backslash, semicolon,
        quote, comma, period, slash,
        /* Special characters */
        space, enter, backspace, tab,
        /* No characters */
        escape, lshfit, rshfit, lctrl, rctrl, lalt, ralt, up, down, left, right,
        /* End Of Enum, just for calculating number of elements of Enum class */
        eoe
    };
    // clang-format on


    struct Event {
        using Clock_t = std::chrono::steady_clock;

        Clock_t::time_point tp_ = Clock_t::now();
        ActionType action_ = ActionType::down;
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

    enum class ActionType { move, down, up, mwheel_up, mwheel_down };

    enum class ButtonCode { left, right, middle, unkown };


    struct Event {
        using Clock_t = std::chrono::steady_clock;

        Clock_t::time_point timepoint_ = Clock_t::now();
        ActionType action_ = ActionType::down;
        ButtonCode button_ = ButtonCode::unkown;
        int32_t btn_id_ = -1;
        double xpos_ = 0;
        double ypos_ = 0;
    };

}}  // namespace sung::mouse
