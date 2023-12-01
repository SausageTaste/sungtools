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
