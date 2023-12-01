#pragma once


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

}
