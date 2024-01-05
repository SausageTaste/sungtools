#include "sung/general/time.hpp"


// TimeChecker
namespace sung {

    double TimeChecker::elapsed() const {
        return this->duration_cast(Clock_t::now() - last_checked_);
    }

    void TimeChecker::set_min() {
        last_checked_ = Clock_t::time_point{};
    }

    void TimeChecker::check() {
        last_checked_ = Clock_t::now();
    }

    double TimeChecker::check_get_elapsed() {
        const auto now = Clock_t::now();
        const auto elapsed = this->duration_cast(now - last_checked_);
        last_checked_ = now;
        return elapsed;
    }

    bool TimeChecker::check_if_elapsed(double seconds) {
        const auto now = Clock_t::now();
        const auto elapsed = this->duration_cast(now - last_checked_);
        if (elapsed >= seconds) {
            last_checked_ = now;
            return true;
        }
        return false;
    }

    double TimeChecker::duration_cast(Clock_t::duration duration) {
        return std::chrono::duration_cast<std::chrono::duration<double>>(duration).count();
    }

}
